# PINBALL-TABLE-HURD #

## INFOS ##

HURD Pinball table for emilia pinball

Another pinball table to have fun, behaviour plugin.
The table is dedicated to GNU/Hurd project.
It features shoot cave, multiballs, ramps and bumpers.

For more details check file:

- [data/hurd/README.txt](data/hurd/README.txt)

## DEMO ##

[![Video](
https://peertube.mastodon.host/static/previews/c72d004d-cc4a-4fe0-a5f0-c064c5e94001.jpg
)](
https://peertube.mastodon.host/videos/embed/c72d004d-cc4a-4fe0-a5f0-c064c5e94001#pinball-table-hurd
)

Watch video on PeerTube:

- <https://peertube.mastodon.host/videos/watch/c72d004d-cc4a-4fe0-a5f0-c064c5e94001#pinball-table-hurd>

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
- <https://github.com/paolo-caroni/pinball/commit/377734778363b9d1397bc645bc4c755a5de66a68>
- <https://mastodon.social/@rzr/103809517516701240#pinball-table-hurd>
