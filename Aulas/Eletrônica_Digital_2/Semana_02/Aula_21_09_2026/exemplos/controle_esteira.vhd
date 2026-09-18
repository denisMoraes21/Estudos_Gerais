library ieee;
use ieee.std_logic_1164.all;
entity controle_esteira is
  port (clk, rst, iniciar, sensor : in std_logic;
        motor, pronto : out std_logic);
end entity;
architecture rtl of controle_esteira is
  type estado_t is (PARADO, MOVENDO, CONCLUIDO);
  signal estado : estado_t;
begin
  process(clk)
  begin
    if rising_edge(clk) then
      if rst = '1' then
        estado <= PARADO;
      else
        case estado is
          when PARADO =>
            if iniciar = '1' then
              estado <= MOVENDO;
            end if;
          when MOVENDO =>
            if sensor = '1' then
              estado <= CONCLUIDO;
            end if;
          when CONCLUIDO =>
            if iniciar = '0' then
              estado <= PARADO;
            end if;
        end case;
      end if;
    end if;
  end process;
  motor  <= '1' when estado = MOVENDO else '0';
  pronto <= '1' when estado = CONCLUIDO else '0';
end architecture;
