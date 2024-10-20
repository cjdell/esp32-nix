# set(COMPONENT_REQUIRES "WiFiManager")

idf_component_register(SRCS "main.cpp" "lib.cpp"
                       INCLUDE_DIRS "."
                      )
