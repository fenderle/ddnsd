# Creating DDNS keys
On the system which should report it's IP address generate the DDNS keypair 
using the following command, replacing ```host.domain.foo``` with the actual 
FQDN of the system.

```
dnssec-keygen -a HMAC-MD5 -b 512 -r /dev/random -n HOST host.domain.foo
```

This will create two files:
- Khost.domain.foo.*.key: this file contains a DNS record for insertion in 
  the DNS zone - we will not need that for this scenario
- Khost.domain.foo.*.private: this file contains all key specific data and
  also the key itself which we are interested in

Use the following command to extract get the key:
```
cat Khost.domain.foo.*.private | grep Key: | cut -f2 -d' '
wyTGnDpoL6RxdppJWM/+wt8vi375z1Fjd/nyKI13CZixecJUfH2yenne1RILxoemPlb3C82mxCWUjvBhbxjvlg==
```

On the DNS server updating the record must be allowed. This is done by adding
to additional blocks to the configuration (named.conf):
```
key host.domain.foo. {
	algorithm HMAC-MD5;
	secret "wyTGnDpoL6RxdppJWM/+wt8vi375z1Fjd/nyKI13CZixecJUfH2yenne1RILxoemPlb3C82mxCWUjvBhbxjvlg==";
};

zone "domain.foo" IN {
	type master;
	file "data/domain.foo.zone";

	update-policy {
		grant host.domain.foo. name host.domain.foo. ANY;
	};
};
```

The ddnsd side needs a configuration like the following to work (ddnsd.conf):
```
[host.domain.foo]
keysecret = wyTGnDpoL6RxdppJWM/+wt8vi375z1Fjd/nyKI13CZixecJUfH2yenne1RILxoemPlb3C82mxCWUjvBhbxjvlg==
```

