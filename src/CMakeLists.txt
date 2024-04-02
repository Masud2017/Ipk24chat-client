file(GLOB SOURCES "*.cc")

# Create library target for files in src directory
add_library(src ${SOURCES})