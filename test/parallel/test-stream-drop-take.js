'use strict';

const common = require('../common');
const {
  Readable,
} = require('stream');
const { deepStrictEqual, rejects, throws } = require('assert');

const { from } = Readable;

const fromAsync = (...args) => from(...args).map(async (x) => x);

const naturals = () => from(async function*() {
  let i = 1;
  while (true) {
    yield i++;
  }
}());

{
  // Synchronous streams
  (async () => {
    deepStrictEqual(await from([1, 2, 3]).drop(2).toArray(), [3]);
    deepStrictEqual(await from([1, 2, 3]).take(1).toArray(), [1]);
    deepStrictEqual(await from([]).drop(2).toArray(), []);
    deepStrictEqual(await from([]).take(1).toArray(), []);
    deepStrictEqual(await from([1, 2, 3]).drop(1).take(1).toArray(), [2]);
    deepStrictEqual(await from([1, 2]).drop(0).toArray(), [1, 2]);
    deepStrictEqual(await from([1, 2]).take(0).toArray(), []);
  })().then(common.mustCall());
  // Asynchronous streams
  (async () => {
    deepStrictEqual(await fromAsync([1, 2, 3]).drop(2).toArray(), [3]);
    deepStrictEqual(await fromAsync([1, 2, 3]).take(1).toArray(), [1]);
    deepStrictEqual(await fromAsync([]).drop(2).toArray(), []);
    deepStrictEqual(await fromAsync([]).take(1).toArray(), []);
    deepStrictEqual(await fromAsync([1, 2, 3]).drop(1).take(1).toArray(), [2]);
    deepStrictEqual(await fromAsync([1, 2]).drop(0).toArray(), [1, 2]);
    deepStrictEqual(await fromAsync([1, 2]).take(0).toArray(), []);
  })().then(common.mustCall());
  // Infinite streams
  // Asynchronous streams
  (async () => {
    deepStrictEqual(await naturals().take(1).toArray(), [1]);
    deepStrictEqual(await naturals().drop(1).take(1).toArray(), [2]);
    const next10 = [11, 12, 13, 14, 15, 16, 17, 18, 19, 20];
    deepStrictEqual(await naturals().drop(10).take(10).toArray(), next10);
    deepStrictEqual(await naturals().take(5).take(1).toArray(), [1]);
  })().then(common.mustCall());
}

{
  // Coercion
  (async () => {
    // The spec made me do this ^^
    deepStrictEqual(await naturals().take('cat').toArray(), []);
    deepStrictEqual(await naturals().take('2').toArray(), [1, 2]);
    deepStrictEqual(await naturals().take(true).toArray(), [1]);
  })().then(common.mustCall());
}

{
  // Support for AbortSignal
  const ac = new AbortController();
  rejects(
    Readable.from([1, 2, 3]).take(1, { signal: ac.signal }).toArray(), {
      name: 'AbortError',
    }).then(common.mustCall());
  rejects(
    Readable.from([1, 2, 3]).drop(1, { signal: ac.signal }).toArray(), {
      name: 'AbortError',
    }).then(common.mustCall());
  ac.abort();
}

{
  // Support for AbortSignal, already aborted
  const signal = AbortSignal.abort();
  rejects(
    Readable.from([1, 2, 3]).take(1, { signal }).toArray(), {
      name: 'AbortError',
    }).then(common.mustCall());
}

{
  // Error cases
  const invalidArgs = [
    -1,
    -Infinity,
    -40,
  ];

  for (const example of invalidArgs) {
    throws(() => from([]).take(example).toArray(), /ERR_OUT_OF_RANGE/);
  }
}
