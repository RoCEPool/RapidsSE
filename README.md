# RapidsSE (mirror)

A copy of **RapidsSE, the Rapids Storage Engine** (Copyright (C) 2026 Boray Data,
Apache License 2.0), from its upstream repository
https://bitbucket.org/rapidsdataproject/rapidsse/ -- every branch, every tag,
the full history, unmodified.

- **The code is in the upstream branches** (e.g. `master`). This branch, `mirror-sync`,
  holds only this note and the job that keeps the copy up to date
  (`.github/workflows/mirror.yml`, daily and on demand).
- **Add-only.** A branch or tag deleted upstream is kept here. A branch whose
  history upstream rewrote keeps its old history; the new one is stored beside
  it as `upstream-rewritten/<date>/<branch>`. A tag that changed upstream is kept
  too; the new one is added as `<tag>-upstream-<date>`. When upstream cannot be
  reached, nothing here changes.
- **Backup.** Every run also attaches the whole repository as one file to the
  release [`backup`](../../releases/tag/backup): `rapidsse.bundle`.
  `git clone rapidsse.bundle rapidsse` restores everything from it.

RapidsSE's license, copyright notice and README are in the upstream branches,
unchanged. rocepool (https://github.com/RoCEPool/rocepool-fs) uses RapidsSE's
WLP v5 client protocol; the upstream files it refers to are also kept in its
`third_party/rapidsse/`.
