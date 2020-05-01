# PINBALL-TABLE-GNU #

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
./debian/rules rule/setup && ./debian/rules && sudo debi
```

Then new table should be listed when you run pinball game.

Also a Dockerfile can be used as reference env.

```sh
docker-compose up --build
```

## RESOURCES ##

- <https://pinball.sf.net>
- <https://github.com/rzr/pinball/issues/4>
- <https://mastodon.social/@rzr/103448015175182101>
