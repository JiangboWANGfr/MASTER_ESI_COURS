add_force {/IP_PWM/clk_avalon} -radix bin {1 0ns} {0 5000ps} -repeat_every 10000ps
add_force {/IP_PWM/rst_avalon} -radix hex {1 0ns}
run 10 ns
add_force {/IP_PWM/rst_avalon} -radix bin {0 0ns}
run 10 ns
add_force {/IP_PWM/cs_avalon} -radix bin {1 0ns}
add_force {/IP_PWM/write_avalon} -radix bin {1 0ns}
add_force {/IP_PWM/address_avalon} -radix bin {10 0ns}
add_force {/IP_PWM/writedata_avalon} -radix bin {1 0ns}
add_force {/IP_PWM/address_avalon} -radix bin {10 0ns}
add_force {/IP_PWM/writedata_avalon} -radix bin {1 0ns}
