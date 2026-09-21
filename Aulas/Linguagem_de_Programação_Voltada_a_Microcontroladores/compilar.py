#!/usr/bin/env python3
"""Compila os oito PDFs sem deixar arquivos auxiliares nas pastas das aulas."""
from pathlib import Path
import shutil
import subprocess
import tempfile


def main():
    root = Path(__file__).resolve().parent
    if not shutil.which("pdflatex"):
        raise SystemExit("Instale uma distribuição LaTeX com pdflatex e Beamer.")
    lessons = sorted(root.glob("Semana_*/Aula_*/main.tex"))
    for source in lessons:
        with tempfile.TemporaryDirectory(prefix="aula-micro-pdf-") as output:
            command = ["pdflatex", "-interaction=nonstopmode", "-halt-on-error",
                       f"-output-directory={output}", "main.tex"]
            for _ in range(2):
                result = subprocess.run(command, cwd=source.parent,
                                        capture_output=True, text=True)
                if result.returncode:
                    raise SystemExit(f"Erro em {source.parent.name}:\n{result.stdout[-6000:]}")
            log = (Path(output) / "main.log").read_text(errors="replace")
            if "Overfull" in log:
                raise SystemExit(f"Conteúdo excede a página em {source.parent}; revise os slides.")
            shutil.copyfile(Path(output) / "main.pdf", source.with_suffix(".pdf"))
        print(f"PDF gerado: {source.parent.relative_to(root)}/main.pdf")
    print(f"{len(lessons)} apresentações compiladas.")


if __name__ == "__main__":
    main()
