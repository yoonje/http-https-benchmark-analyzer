# Http Https Benchmark Analyzer
Analyzer that analyzes the difference in latency between http and https. Generally http more faster than https.
![result](https://user-images.githubusercontent.com/38535571/83666107-5e949880-a607-11ea-980e-c50d48ebc9fc.PNG)

### Requirement
* Ubuntu 18.04

### Installation
```sh
$ sudo apt install –y make libssl1.0-dev
```
```sh
$ git clone https://github.com/yoonje/http-https-benchmark-analyzer
```

### Build
```sh
$ sh build.sh
```

### Run
```sh
$ sh http-https-benchmarker.sh {target address}
```
