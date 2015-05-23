
> This is a rewrite using native C++, ldns, pcre and curl

# ddnsd - A simple DDNS updater
This is a little tool which helps keeping DDNS records up to date. This is
not to be confused with the well known Dynamic DNS services like dyn.com or
others.

Furthermore this tool is used with DDNS capable DNS servers. However the
mechanics are the same. A daemon is started which check periodically for
a change of the external IP, usually by pulling some webpage which returns
the IP address of the requesting client. Then this IP address is checked
against a DNS A record and if required an update is attempted on that 
record.

## Installation
The software is expected to build on RHEL7 and it's derivates. It requires
the EPEL repository for the perl-Config-IniFiles dependency. For 
installation download the release tar and execute the following statement:

```
rpmbuild -ta ddnsd-*.tar.gz
```

Of course this requires you to have setup a proper rpmbuild environment.
After execution of the above command a rpm file will be dropped in the
appropriate place which then can be installed on the target system.

For distributions other than RHEL7 the installation is left to the user.
However I'm happy to accept merge requests for deb packaging or others.

## Configuring the DNS zone
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

## Configuring ddnsd
The ddnsd side needs a configuration like the following to work (ddnsd.conf):
```
[host.domain.foo]
keysecret = wyTGnDpoL6RxdppJWM/+wt8vi375z1Fjd/nyKI13CZixecJUfH2yenne1RILxoemPlb3C82mxCWUjvBhbxjvlg==
```

For a reference checkout the man page.

## Running ddnsd
ddnsd is integrated with systemd and NetworkManager. You can use the usual systemctl commands to
start, stop, enable or disable ddnsd. If ddnsd is enabled and NetworkManager detects an change on the
interface running the default gateway it will also restart ddnsd automatically (useful for ppp or
other dhcp based environments).

