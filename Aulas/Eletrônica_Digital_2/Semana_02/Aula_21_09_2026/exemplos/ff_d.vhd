library ieee;
use ieee.std_logic_1164.all;
entity ff_d is
  port (clk, rst, d : in std_logic;
        q : out std_logic);
end entity;
architecture rtl of ff_d is
begin
  process(clk)
  begin
    if rising_edge(clk) then
      if rst = '1' then q <= '0';
      else q <= d;
      end if;
    end if;
  end process;
end architecture;
