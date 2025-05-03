docker build -t build-max-test .

mkdir -p build

docker run --rm -v ./build:/out -v ./program:/program build-max-test