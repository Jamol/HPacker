# HPacker
```
A HPACK lib implemented in c++
```

### Usage
```c
HPacker::KeyValueVector headers;
headers.push_back(make_pair(":authority", "www.google.com"));
headers.push_back(make_pair(":method", "GET"));
headers.push_back(make_pair(":path", "/"));

uint8_t buf[1024];
HPacker hpe;
int ret = hpe.encode(headers, buf, len);
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


