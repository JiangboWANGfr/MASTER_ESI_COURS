library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity IP_PWM is
    port (
        --------------------------------------------------------------------
        -- Avalon
        --------------------------------------------------------------------
        clk_avalon : in std_logic;
        rst_avalon : in std_logic;
        cs_avalon : in std_logic;
        write_avalon : in std_logic;
        address_avalon : in std_logic_vector(1 downto 0);
        writedata_avalon : in std_logic_vector(31 downto 0);
        read_avalon : in std_logic;
        readdata_avalon : out std_logic_vector(31 downto 0);

        --------------------------------------------------------------------
        -- PWM
        --------------------------------------------------------------------
        pwm : out std_logic
    );

end entity;

architecture behav of IP_PWM is
    signal registre_CRTL : std_logic_vector(31 downto 0);
    signal registre_N : std_logic_vector(31 downto 0);
    signal registre_status : std_logic_vector(31 downto 0);
    signal registre_sigma : std_logic_vector(31 downto 0);

    signal clk, reset, enable_cpt, enable_chip : std_logic;
    signal q : integer;
    signal prescaler_clk : std_logic;
    signal counter_val : std_logic_vector(31 downto 0);
    signal pwm_signal : std_logic;

begin

    --------------------------------------------------------------------
    -- Interface avalon - IP
    --------------------------------------------------------------------
    clk <= clk_avalon;
    reset <= registre_CRTL(0);
    enable_cpt <= registre_CRTL(1);
    enable_chip <= registre_CRTL(2);

    process (clk_avalon)
    begin
        if rst_avalon = '1' then
            registre_CRTL <= (others => '0');
            registre_N <= (others => '0');
            registre_status <= (others => '0');
            registre_sigma <= (others => '0');
        elsif rising_edge(clk_avalon) then
            if cs_avalon = '1' then
                if read_avalon = '1' then
                    case address_avalon is
                        when "00" => readdata_avalon <= registre_N;
                        when "01" => readdata_avalon <= registre_sigma;
                        when "10" => readdata_avalon <= registre_CRTL;
                        when "11" => readdata_avalon <= registre_status;
                        when others => readdata_avalon <= (others => '0');
                    end case;
                elsif write_avalon = '1' then
                    case address_avalon is
                        when "00" => registre_N <= writedata_avalon;
                        when "01" => registre_sigma <= writedata_avalon;
                        when "10" => registre_CRTL <= writedata_avalon;
                        when "11" => registre_status <= writedata_avalon;
                        when others => registre_N <= writedata_avalon;
                    end case;
                end if;
            end if;
        end if;
    end process;

    -- Instantiate Prescaler
    prescaler_inst : entity work.Prescaler
        port map(
            clk_in => clk_avalon,
            reset => rst_avalon,
            enable => enable_cpt,
            scale_factor => registre_N,
            clk_out => prescaler_clk
        );

    -- Instantiate Counter
    counter_inst : entity work.Counter
        port map(
            clk => prescaler_clk,
            reset => rst_avalon,
            enable => enable_cpt,
            counter_out => counter_val
        );

    -- Instantiate Comparator
    comparator_inst : entity work.Comparator
        port map(
            counter_value => counter_val,
            compare_value => registre_sigma,
            pwm_out => pwm
        );
end;

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity Prescaler is
    port (
        clk_in : in std_logic;
        reset : in std_logic;
        enable : in std_logic;
        scale_factor : in std_logic_vector(31 downto 0);
        clk_out : buffer std_logic
    );
end entity;

architecture Behavioral of Prescaler is
    signal prescale_counter : std_logic_vector(31 downto 0);
begin
    process (clk_in, reset)
    begin
        if reset = '1' then
            prescale_counter <= (others => '0');
            clk_out <= '0';
        elsif rising_edge(clk_in) and enable = '1' then
            if prescale_counter = scale_factor then
                prescale_counter <= (others => '0');
                clk_out <= not clk_out;
            else
                prescale_counter <= prescale_counter + 1;
            end if;
        end if;
    end process;
end;

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity Counter is
    port (
        clk : in std_logic;
        reset : in std_logic;
        enable : in std_logic;
        counter_out : buffer std_logic_vector(7 downto 0)
    );
end entity;

architecture Behavioral of Counter is
begin
    process (clk, reset)
    begin
        if reset = '1' then
            counter_out <= (others => '0');
        elsif rising_edge(clk) and enable = '1' then
            if counter_out = "11111111" then
                counter_out <= (others => '0');
            else
                counter_out <= counter_out + 1;
            end if;
        end if;
    end process;
end;
library IEEE;
use IEEE.std_logic_1164.all;

entity Comparator is
    port (
        counter_value : in std_logic_vector(31 downto 0);
        compare_value : in std_logic_vector(31 downto 0);
        pwm_out : out std_logic
    );
end entity;

architecture Behavioral of Comparator is
begin
    process (counter_value, compare_value)
    begin
        if counter_value <= compare_value then
            pwm_out <= '1';
        else
            pwm_out <= '0';
        end if;
    end process;
end;