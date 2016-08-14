/*
* FileSystemWatcherDemo.cs: a demo for use FileSystemWatcher to watch the change of file in Windows
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.IO;
using System.Security.Permissions;

namespace WatchFile
{
	class Program
	{
		public static AutoResetEvent s_event;

		static void Main(string[] args)
		{
			s_event = new AutoResetEvent(false);

			Thread t = new Thread(Watcher);
			t.IsBackground = true;
			t.Start();

			Console.ReadKey();
			s_event.Set();
		}


		[PermissionSet(SecurityAction.Demand, Name = "FullTrust")]
		static void Watcher()
		{
			string[] paths = System.Environment.GetCommandLineArgs();

			if (paths.Length <= 1)
			{
				Console.WriteLine("give a folder to watch");
				Thread.Sleep(2000);
				return;
			}

			for (int i = 1; i < paths.Length; i++)
			{
				if (!Directory.Exists(paths[i]))
					continue;
				FileSystemWatcher fsw = new FileSystemWatcher();
				fsw.Path = paths[i];
				fsw.NotifyFilter = NotifyFilters.FileName | NotifyFilters.LastAccess | NotifyFilters.DirectoryName | NotifyFilters.LastWrite
					| NotifyFilters.Size | NotifyFilters.CreationTime | NotifyFilters.Attributes;
				fsw.Changed += OnChanged;
				fsw.Created += OnChanged;
				fsw.Deleted += OnChanged;

				fsw.Error += OnError;

				fsw.Renamed += OnRename;
				fsw.Filter = "";

				fsw.EnableRaisingEvents = true;


			}

			s_event.WaitOne();
		}

		private static void OnChanged(object source, FileSystemEventArgs e)
		{
			// Specify what is done when a file is changed, created, or deleted.
			Console.WriteLine("File: " + e.FullPath + " " + e.ChangeType);
		}

		private static void OnError(object sender, ErrorEventArgs e)
		{
			Console.WriteLine("Watcher error: " + e.GetException().StackTrace + " " + e.GetException().Message);
		}

		private static void OnRename(object sender, RenamedEventArgs e)
		{
			Console.WriteLine(string.Format("Rename: before: {0}, {1}, after: {2}, {3}", e.OldFullPath, e.OldName, e.FullPath, e.Name));
		}
	}
}
