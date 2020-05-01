# PINBALL-TABLE-GNU #

[![GitHub forks](
https://img.shields.io/github/forks/rzr/pinball-table-gnu.svg?style=social&label=Fork&maxAge=2592000
)](
https://github.com/rzr/pinball-table-gnu
)
[![Fediverse](
https://mastodon.social/@rzr/103765397111911239#pinball-table-gnu#
)](
https://img.shields.io/mastodon/follow/279303?domain=https%3A%2F%2Fmastodon.social&style=social#
)

## INFOS ##

GNU Pinball table for emilia pinball

Another pinball table to have fun, behaviour plugin.
The table is dedicated to GNU project.
It features magnets and bumpers.

For more details check file:

- [data/gnu/README](data/gnu/README)

## DEMO ##

[![Video](
https://peertube.mastodon.host/static/previews/35d08be9-0b05-4477-b6c5-d1cc95890b66.jpg
)](
https://peertube.mastodon.host/videos/embed/35d08be9-0b05-4477-b6c5-d1cc95890b66#pinball-table-gnu
)

Watch video on PeerTube:

- <https://peertube.mastodon.host/videos/watch/35d08be9-0b05-4477-b6c5-d1cc95890b66#pinball-table-gnu>

## USAGE ##

It's easy build table assuming that Emilia pinball headers installed in system.

```sh
./bootstrap && ./configure && make && make install
```

For Debian users building a package can be automated using:

```sh
./debian/Makefile rule/setup && ./debian/Makefile && sudo debi
```

Then new table should be listed when you run pinball game.

Also a Dockerfile can be used as reference env.

```sh
docker-compose up --build
```

## RESOURCES ##

- <https://pinball.sf.net>
- <https://purl.org/rzr/pinball>
- <https://github.com/rzr/pinball/issues/4>
- <https://mastodon.social/@rzr/103448015175182101>
