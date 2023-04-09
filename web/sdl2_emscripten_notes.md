SDL2 and Emscripten
===================

1. Install Emscripten SDK following this manual:

	https://emscripten.org/docs/getting_started/downloads.html

2. Use following command to build WASM-executables:

	```bash
	source /opt/emsdk/emsdk_env.sh
	
	mkdir build
	cd build
	
	emcmake cmake -DBACKEND=SDL2 ..
	make
	```

3. Copy all necessary files:

	```bash
	mkdir perlin_test
	cp ../web/index.html perlin_test
	cp perlin_test.js perlin_test
	cp perlin_test.wasm perlin_test
	```

4. Deploy `perlin_test` directory to Web-server.
