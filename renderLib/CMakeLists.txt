add_library(renderLib
	FrameBuffer.cpp FrameBuffer.hpp
)

add_executable(main
	main.cpp
)
target_link_libraries(main PRIVATE renderLib)
target_link_libraries(main PRIVATE cs4212-util)
target_link_libraries(main PRIVATE ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(main PRIVATE PNG::PNG)
target_link_libraries(main PRIVATE ZLIB::ZLIB)

