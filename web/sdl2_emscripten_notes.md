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

	emstrip -s *.wasm
	```

3. Copy all necessary files:

	```bash
	mkdir perlin_test
	cp ../web/index_perlin_test.html perlin_test/index.html
	cp perlin_test.js perlin_test
	cp perlin_test.wasm perlin_test

	mkdir snake_pit
	cp ../web/index_snake_pit.html snake_pit/index.html
	cp snake_pit.js snake_pit
	cp snake_pit.wasm snake_pit
	```

4. Deploy `perlin_test` and `snake_pit` directories to Web-server.

	### Web
	
	Add a MIME type for WASM binaries to serve files properly, additional information is here: [Hosting a WebAssembly App](https://platform.uno/docs/articles/how-to-host-a-webassembly-app.html).
	
	#### Nginx
	
	Add this to the http section of the configuration file:
	
	```nginx
	types {
		application/wasm wasm;
	}
	```
	
	#### Apache
	
	Add this to the apache2.conf or httpd.conf configuration files:
	
	```apache
	AddType application/wasm .wasm
	```

5. Enjoy!
