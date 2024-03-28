idf_component_register(
        SRCS "src/collections.c" "src/dump.c" "src/mpi.c"
        REQUIRES mbedtls
        INCLUDE_DIRS "include")
