CXX=g++
CXXFLAGS=-Wextra -Wpedantic -Wall -Werror=vla -MMD -g 

OBJECTS = main.o model.o view.o controller.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = wordle

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
.PHONY: clean
