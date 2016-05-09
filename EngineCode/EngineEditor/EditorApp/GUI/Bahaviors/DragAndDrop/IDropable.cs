using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EditorApp.GUI
{
	/// <summary>
	/// http://www.codeproject.com/Articles/420545/WPF-Drag-and-Drop-MVVM-using-Behavior
	/// License: The Code Project Open License (CPOL) 1.02
	/// http://www.codeproject.com/info/cpol10.aspx
	/// </summary>

	interface IDropable
    {
        /// <summary>
        /// Type of the data item
        /// </summary>
        Type DataType { get; }

        /// <summary>
        /// Drop data into the collection.
        /// </summary>
        /// <param name="data">The data to be dropped</param>
        /// <param name="index">optional: The index location to insert the data</param>
        void Drop(object data, int index = -1);
    }
}
