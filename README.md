# HPacker
```
A HPACK lib implemented in c++
```

### Usage
```c
#include "HPacker.h"
using namespace hpack;

HPacker::KeyValueVector headers;
headers.emplace_back(":authority", "www.google.com");
headers.emplace_back(":method", "GET");
headers.emplace_back(":path", "/");

uint8_t buf[1024];
HPacker hpe;
int ret = hpe.encode(headers, buf, sizeof(buf));
if (ret < 0) {
	printf("encode failed\n");
	return 1;
}
printf("encoded_len=%d\n", ret);

headers.clear();
HPacker hpd;
ret = hpd.decode(buf, ret, headers);
if (ret < 0) {
	printf("decode failed\n");
	return 1;
}
printf("decoded_len=%d\n", ret);
```

### LICENSE
MIT


