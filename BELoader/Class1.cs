using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;

namespace BELoader
{
    public sealed class MinGapPacer
    {
        private readonly long _gapTicks;
        private long _nextEligibleTicks; // 0 = immediately

        public MinGapPacer(TimeSpan minGap)
        {
            _gapTicks = (long)(Stopwatch.Frequency * minGap.TotalSeconds);
            _nextEligibleTicks = 0;
        }

        // Call before each send (blocking)
        public void Wait()
        {
            long now = Stopwatch.GetTimestamp();
            long deadline = (_nextEligibleTicks == 0) ? now : _nextEligibleTicks;

            SleepUntil(deadline);                    // block until eligible
            _nextEligibleTicks = Stopwatch.GetTimestamp() + _gapTicks; // set next window
        }

        // Call before each send (async)
        public async Task WaitAsync(CancellationToken ct = default)
        {
            long now = Stopwatch.GetTimestamp();
            long deadline = (_nextEligibleTicks == 0) ? now : _nextEligibleTicks;

            await SleepUntilAsync(deadline, ct);
            _nextEligibleTicks = Stopwatch.GetTimestamp() + _gapTicks;
        }

        private static void SleepUntil(long deadline)
        {
            // Coarse sleep, then short spin for sub-ms precision
            while (true)
            {
                long now = Stopwatch.GetTimestamp();
                long remaining = deadline - now;
                if (remaining <= 0) return;

                double ms = remaining * 1000.0 / Stopwatch.Frequency;
                if (ms > 1.5)
                    Thread.Sleep((int)(ms - 0.75)); // leave ~0.75ms for the spin
                else
                    SpinWait.SpinUntil(() => Stopwatch.GetTimestamp() >= deadline);
            }
        }

        private static async Task SleepUntilAsync(long deadline, CancellationToken ct)
        {
            while (true)
            {
                long now = Stopwatch.GetTimestamp();
                long remaining = deadline - now;
                if (remaining <= 0) return;

                double ms = remaining * 1000.0 / Stopwatch.Frequency;
                if (ms > 1.5)
                    await Task.Delay((int)(ms - 0.75), ct);
                else
                {
                    await Task.Yield(); // keep UI/reactivity
                    SpinWait.SpinUntil(() => Stopwatch.GetTimestamp() >= deadline);
                }
            }
        }
    }

}
