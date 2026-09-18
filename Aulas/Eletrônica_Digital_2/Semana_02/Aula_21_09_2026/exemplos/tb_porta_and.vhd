library ieee;
use ieee.std_logic_1164.all;
entity tb_porta_and is
end entity;
architecture sim of tb_porta_and is
  signal a, b : std_logic := '0';
  signal y : std_logic;
begin
  dut: entity work.porta_and(rtl)
    port map (a => a, b => b, y => y);
  estimulos: process
  begin
    a <= '0'; b <= '0'; wait for 10 ns;
    assert y = '0' report "Falha: 00" severity error;
    a <= '0'; b <= '1'; wait for 10 ns;
    assert y = '0' report "Falha: 01" severity error;
    a <= '1'; b <= '0'; wait for 10 ns;
    assert y = '0' report "Falha: 10" severity error;
    a <= '1'; b <= '1'; wait for 10 ns;
    assert y = '1' report "Falha: 11" severity error;
    report "Teste concluido: 4 casos" severity note;
    std.env.finish;
    wait;
  end process;
end architecture;
