idf_component_register(SRCS "plant_faces.c" "play_game.c" "pet_game.c" "interaction.c" "light_sensor.c" "ultrasonic.c" "i2c.c" "display.c" "main.c" "buttons.c" "plant.c"
                    INCLUDE_DIRS "."
                    REQUIRES driver esp_timer
)