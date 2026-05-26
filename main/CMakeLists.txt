idf_component_register(SRCS "i2c.c" "display.c" "main.c" "dice.c" "buttons.c"
                    INCLUDE_DIRS "."
                    REQUIRES driver
)