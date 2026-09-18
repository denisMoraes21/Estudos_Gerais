library ieee;
use ieee.std_logic_1164.all;
entity mux2 is
  port (a, b, sel : in std_logic;
        y : out std_logic);
end entity;
architecture rtl of mux2 is
begin
  y <= a when sel = '0' else b;
end architecture;
