all: build

build: harness.exe extractor.exe

harness.exe: harness.cpp
	clang++ --target=x86_64-w64-windows-gnu -static-libgcc -static-libstdc++ $^ -o $@

extractor.exe: extractor.cpp
	clang++ --target=x86_64-w64-windows-gnu -static-libgcc -static-libstdc++ $^ -o $@ -ldbgeng

clean:
	rm -rf harness.exe extractor.exe

rebuild: clean build