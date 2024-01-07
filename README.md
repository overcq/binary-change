# binary-change

Programs for reading and writing variables to a binary file:

* binary-read
* binary-write

Available options after specifying the `-?` argument.

In short:

* `binary-read -f <pathname> -p <offset> [ -l | -b ] -s <bytes> ...`
* `binary-write -f <pathname> -p <offset> [ -l | -b ] -s <bytes> -d <value> ...`

# Information and instructions to the user

## 1. Contact

Manufacturer: overcq

Contact the manufacturer: overcq@int.pl

Source code: https://github.com/overcq/binary-change

## 2. Vulnerabilities

Report vulnerabilities: https://github.com/overcq/binary-change/security/advisories or via e‐mail: overcq@int.pl

## 3. Version

Detailed version in git log.

## 4. Purpose

The product is intended for software development in unix-like systems.

## 5. Risk

The use of these programs requires knowledge of the binary structure of the file being modified or read.

## 6. Component materials

This product uses external libraries (source code available through the operating systemʼs package system):

* “libc”

This product has been built using unix-like tools, in particular:

* “clang” or “gcc”
* “install”
* GNU “make”
* “rm”
* shell

## 7. EU declaration

No the EU declaration of conformity.

## 8. Support

Technical security support via e‐mail: overcq@int.pl

Issues at: https://github.com/overcq/binary-change/issues

Product support continues for the latest git release.

## 9. Installation and update

Fetching for first use can be done with a command, for example `git clone git@github.com:overcq/binary-change.git`.

The user should make regular updates, e.g. with a command `git pull && make`, then as “root”: `make install`.
Before using this commands can be executed as “root”: `make uninstall` and/or as the user: `make clean`, to remove any remaining intermediate files that depend on source files that have been removed in the new product version.

To completely remove the product from the userʼs device, execute, for example, as “root”: `make uninstall`, then as the user: `rm -fr binary-change`.
