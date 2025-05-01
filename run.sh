docker build -t build-max-test .

mkdir build

docker run --rm -v ./build:/out -v ./program:/program build-max-test