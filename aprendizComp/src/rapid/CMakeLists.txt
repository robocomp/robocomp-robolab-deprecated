cmake_minimum_required(VERSION 2.6)
PROJECT( librapid )
INCLUDE_DIRECTORIES(${APRENDIZ})

SET (LIBRARY_OUTPUT_PATH ${APRENDIZ}/lib)
# ADD library
ADD_LIBRARY ( rapid
	STATIC
	rapid.C 
	build.C 
	collide.C 
	overlap.C
)



