# Node.js 17 ChangeLog

<!--lint disable prohibited-strings-->
<!--lint disable maximum-line-length-->
<!--lint disable no-literal-urls-->

<table>
<tr>
<th>Current</th>
</tr>
<tr>
<td>
<a href="#17.0.0">17.0.0</a><br/>
</td>
</tr>
</table>

* Other Versions
  * [16.x](CHANGELOG_V16.md)
  * [15.x](CHANGELOG_V15.md)
  * [14.x](CHANGELOG_V14.md)
  * [13.x](CHANGELOG_V13.md)
  * [12.x](CHANGELOG_V12.md)
  * [11.x](CHANGELOG_V11.md)
  * [10.x](CHANGELOG_V10.md)
  * [9.x](CHANGELOG_V9.md)
  * [8.x](CHANGELOG_V8.md)
  * [7.x](CHANGELOG_V7.md)
  * [6.x](CHANGELOG_V6.md)
  * [5.x](CHANGELOG_V5.md)
  * [4.x](CHANGELOG_V4.md)
  * [0.12.x](CHANGELOG_V012.md)
  * [0.10.x](CHANGELOG_V010.md)
  * [io.js](CHANGELOG_IOJS.md)
  * [Archive](CHANGELOG_ARCHIVE.md)

<a id="17.0.0"></a>
## 2021-10-19, Version 17.0.0 (Current), @BethGriggs

### Notable Changes

#### Deprecations and Removals

* TBD

#### Other Notable Changes

### Semver-Major Commits

* [[`40c6e838df`](https://github.com/nodejs/node/commit/40c6e838df)] - **(SEMVER-MAJOR)** **dgram**: tighten `address` validation in `socket.send` (Voltrex) [#39190](https://github.com/nodejs/node/pull/39190)
* [[`f182b9b29f`](https://github.com/nodejs/node/commit/f182b9b29f)] - **(SEMVER-MAJOR)** **dns**: runtime deprecate type coercion of `dns.lookup` options (Antoine du Hamel) [#39793](https://github.com/nodejs/node/pull/39793)
* [[`1b2749ecbe`](https://github.com/nodejs/node/commit/1b2749ecbe)] - **(SEMVER-MAJOR)** **dns**: default to verbatim=true in dns.lookup() (treysis) [#39987](https://github.com/nodejs/node/pull/39987)
* [[`ae876d420c`](https://github.com/nodejs/node/commit/ae876d420c)] - **(SEMVER-MAJOR)** **doc**: update minimum supported FreeBSD to 12.2 (Michaël Zasso) [#40179](https://github.com/nodejs/node/pull/40179)
* [[`59d3d542d6`](https://github.com/nodejs/node/commit/59d3d542d6)] - **(SEMVER-MAJOR)** **errors**: disp ver on fatal except that causes exit (Divlo) [#38332](https://github.com/nodejs/node/pull/38332)
* [[`f9447b71a6`](https://github.com/nodejs/node/commit/f9447b71a6)] - **(SEMVER-MAJOR)** **fs**: fix rmsync error swallowing (Nitzan Uziely) [#38684](https://github.com/nodejs/node/pull/38684)
* [[`f27b7cf95c`](https://github.com/nodejs/node/commit/f27b7cf95c)] - **(SEMVER-MAJOR)** **fs**: aggregate errors in fsPromises to avoid error swallowing (Nitzan Uziely) [#38259](https://github.com/nodejs/node/pull/38259)
* [[`d0a898681f`](https://github.com/nodejs/node/commit/d0a898681f)] - **(SEMVER-MAJOR)** **lib**: add structuredClone() global (Ethan Arrowood) [#39759](https://github.com/nodejs/node/pull/39759)
* [[`e4b1fb5e64`](https://github.com/nodejs/node/commit/e4b1fb5e64)] - **(SEMVER-MAJOR)** **lib**: expose `DOMException` as global (Khaidi Chu) [#39176](https://github.com/nodejs/node/pull/39176)
* [[`36e2ffe6dc`](https://github.com/nodejs/node/commit/36e2ffe6dc)] - **(SEMVER-MAJOR)** **module**: subpath folder mappings EOL (Guy Bedford) [#40121](https://github.com/nodejs/node/pull/40121)
* [[`64287e4d45`](https://github.com/nodejs/node/commit/64287e4d45)] - **(SEMVER-MAJOR)** **module**: runtime deprecate trailing slash patterns (Guy Bedford) [#40117](https://github.com/nodejs/node/pull/40117)
* [[`707dd77d86`](https://github.com/nodejs/node/commit/707dd77d86)] - **(SEMVER-MAJOR)** **readline**: validate `AbortSignal`s and remove unused event listeners (Antoine du Hamel) [#37947](https://github.com/nodejs/node/pull/37947)
* [[`8122d243ae`](https://github.com/nodejs/node/commit/8122d243ae)] - **(SEMVER-MAJOR)** **readline**: introduce promise-based API (Antoine du Hamel) [#37947](https://github.com/nodejs/node/pull/37947)
* [[`592d1c3d44`](https://github.com/nodejs/node/commit/592d1c3d44)] - **(SEMVER-MAJOR)** **readline**: refactor `Interface` to ES2015 class (Antoine du Hamel) [#37947](https://github.com/nodejs/node/pull/37947)
* [[`3f619407fe`](https://github.com/nodejs/node/commit/3f619407fe)] - **(SEMVER-MAJOR)** **src**: allow CAP\_NET\_BIND\_SERVICE in SafeGetenv (Daniel Bevenius) [#37727](https://github.com/nodejs/node/pull/37727)
* [[`0a7f850123`](https://github.com/nodejs/node/commit/0a7f850123)] - **(SEMVER-MAJOR)** **src**: return Maybe from a couple of functions (Darshan Sen) [#39603](https://github.com/nodejs/node/pull/39603)
* [[`bdaf51bae7`](https://github.com/nodejs/node/commit/bdaf51bae7)] - **(SEMVER-MAJOR)** **src**: allow custom PageAllocator in NodePlatform (Shelley Vohr) [#38362](https://github.com/nodejs/node/pull/38362)
* [[`0c6f345cda`](https://github.com/nodejs/node/commit/0c6f345cda)] - **(SEMVER-MAJOR)** **stream**: fix highwatermark threshold and add the missing error (Rongjian Zhang) [#38700](https://github.com/nodejs/node/pull/38700)
* [[`0e841b45c2`](https://github.com/nodejs/node/commit/0e841b45c2)] - **(SEMVER-MAJOR)** **stream**: don't emit 'data' after 'error' or 'close' (Robert Nagy) [#39639](https://github.com/nodejs/node/pull/39639)
* [[`ef992f6de9`](https://github.com/nodejs/node/commit/ef992f6de9)] - **(SEMVER-MAJOR)** **stream**: do not emit `end` on readable error (Szymon Marczak) [#39607](https://github.com/nodejs/node/pull/39607)
* [[`efd40eadab`](https://github.com/nodejs/node/commit/efd40eadab)] - **(SEMVER-MAJOR)** **stream**: forward errored to callback (Robert Nagy) [#39364](https://github.com/nodejs/node/pull/39364)
* [[`09d8c0c8d2`](https://github.com/nodejs/node/commit/09d8c0c8d2)] - **(SEMVER-MAJOR)** **stream**: destroy readable on read error (Robert Nagy) [#39342](https://github.com/nodejs/node/pull/39342)
* [[`a5dec3a470`](https://github.com/nodejs/node/commit/a5dec3a470)] - **(SEMVER-MAJOR)** **stream**: validate abort signal (Robert Nagy) [#39346](https://github.com/nodejs/node/pull/39346)
* [[`bb275ef2a4`](https://github.com/nodejs/node/commit/bb275ef2a4)] - **(SEMVER-MAJOR)** **stream**: unify stream utils (Robert Nagy) [#39294](https://github.com/nodejs/node/pull/39294)
* [[`b2ae12d422`](https://github.com/nodejs/node/commit/b2ae12d422)] - **(SEMVER-MAJOR)** **stream**: throw on premature close in Readable\[AsyncIterator\] (Darshan Sen) [#39117](https://github.com/nodejs/node/pull/39117)
* [[`0738a2b7bd`](https://github.com/nodejs/node/commit/0738a2b7bd)] - **(SEMVER-MAJOR)** **stream**: finished should error on errored stream (Robert Nagy) [#39235](https://github.com/nodejs/node/pull/39235)
* [[`954217adda`](https://github.com/nodejs/node/commit/954217adda)] - **(SEMVER-MAJOR)** **stream**: error Duplex write/read if not writable/readable (Robert Nagy) [#34385](https://github.com/nodejs/node/pull/34385)
* [[`f4609bdf3f`](https://github.com/nodejs/node/commit/f4609bdf3f)] - **(SEMVER-MAJOR)** **stream**: bypass legacy destroy for pipeline and async iteration (Robert Nagy) [#38505](https://github.com/nodejs/node/pull/38505)
* [[`e1e669b109`](https://github.com/nodejs/node/commit/e1e669b109)] - **(SEMVER-MAJOR)** **url**: throw invalid this on detached accessors (James M Snell) [#39752](https://github.com/nodejs/node/pull/39752)
* [[`70157b9cb7`](https://github.com/nodejs/node/commit/70157b9cb7)] - **(SEMVER-MAJOR)** **url**: forbid certain confusable changes from being introduced by toASCII (Timothy Gu) [#38631](https://github.com/nodejs/node/pull/38631)

### Semver-Minor Commits

* [[`341312d78a`](https://github.com/nodejs/node/commit/341312d78a)] - **(SEMVER-MINOR)** **readline**: add `autoCommit` option (Antoine du Hamel) [#37947](https://github.com/nodejs/node/pull/37947)

### Semver-Patch Commits

* TBD
