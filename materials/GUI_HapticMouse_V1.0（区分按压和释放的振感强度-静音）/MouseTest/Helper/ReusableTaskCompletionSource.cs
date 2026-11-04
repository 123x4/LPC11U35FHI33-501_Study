using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MouseTest.Helper
{
    public class ReusableTaskCompletionSource<T>
    {
        private TaskCompletionSource<T> _tcs;

        public ReusableTaskCompletionSource()
        {
            _tcs = new TaskCompletionSource<T>();
        }

        // 获取当前任务
        public Task<T> Task => _tcs.Task;

        // 设置任务的结果，并重置 TaskCompletionSource 以允许重新使用
        public void SetResult(T result)
        {
            _tcs.SetResult(result);
            _tcs = new TaskCompletionSource<T>();  // 重新初始化，允许下一次使用
        }

        // 设置任务为失败
        public void SetException(Exception exception)
        {
            _tcs.SetException(exception);
            _tcs = new TaskCompletionSource<T>();  // 重新初始化，允许下一次使用
        }

        // 设置任务为取消
        public void SetCanceled()
        {
            _tcs.SetCanceled();
            _tcs = new TaskCompletionSource<T>();  // 重新初始化，允许下一次使用
        }
    }
}
