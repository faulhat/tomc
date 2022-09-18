antlr:
	bash -c "\
		mkdir -p grammar; \
		cd src/antlr4/; \
		antlr4 -Dlanguage=Cpp -o ../../grammar/ CProgram.g4"
