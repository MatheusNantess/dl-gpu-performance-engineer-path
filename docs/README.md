# Docs

```text
concepts/                # explanations of ideas, written to be read by someone else
writeups/                # the technical write-ups -- the portfolio-facing output
benchmark-methodology/   # how every measurement in this repo is made
hardware/                # device records; every result references one
templates/               # skeletons for write-ups, benchmark reports, profiling reports
```

## Write-ups are the point

> Every important concept should eventually become code, a benchmark, a profiling experiment, or a technical write-up.

`writeups/` is where the last of those lives, and it is what a reader is actually pointed at. A good one has: the operation, the derivation, the implementation approach, the profiler evidence, the optimization attempts including the failed ones, and honest numbers with hardware attached.

Being able to explain a kernel is a large part of the job. A kernel nobody can follow is worth less than a slightly slower one with a clear write-up.

## Standard

- No invented, projected or extrapolated numbers.
- Every measurement references a hardware record in `hardware/`.
- Failed attempts appear. They are usually the most informative part.
- Open questions are stated as open, not smoothed over.