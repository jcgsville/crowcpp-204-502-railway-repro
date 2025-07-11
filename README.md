# crowcpp-204-502-railway-repro

Build native:

```sh
cmake -S . -B .build && cmake --build .build --parallel 10
```

Run native:

```sh
.build/server
```

Build docker image for running on MacOS w Apple Silicon:

```sh
docker buildx build --load --platform linux/arm64 -t crowcpp-204-502-railway-repro .
```

Run docker container:

```sh
docker run --rm -p 8080:8080 -e PORT=8080 --name crowcpp-204-502-railway-repro crowcpp-204-502-railway-repro
```

Test OPTIONS request locally:

```sh
curl -is --raw -X OPTIONS "http://localhost:8080/hello" -H "Origin: https://any-origin.com" -H "Access-Control-Request-Method: POST" -H "Access-Control-Request-Headers: some-custom-header"
```

This is what I get when I send the OPTIONS request to a native local server:

```
HTTP/1.1 204 No Content
Access-Control-Allow-Headers: content-type, some-custom-header
Access-Control-Allow-Methods: POST, OPTIONS
Allow: OPTIONS, HEAD, POST
```

This is what I get when I send the OPTIONS request to a locally running docker container:

```
HTTP/1.1 204 No Content
Access-Control-Allow-Headers: content-type, some-custom-header
Access-Control-Allow-Methods: POST, OPTIONS
Access-Control-Allow-Origin: http://any-origin.com
```

Bizarre that they are different, but that shouldn't be the source of Railway turning 204s to 502s, so onward.
