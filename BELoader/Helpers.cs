using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.LinkLabel;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace BELoader
{

    static public class Helpers
    {



        /// <summary>
        /// Assigns a tooltip with multiline text to the given control.
        /// </summary>
        /// <param name="control">The control to attach the tooltip to.</param>
        /// <param name="tooltipText">The tooltip text (may contain line breaks).</param>
        public static void SetMultilineToolTip(Control control, List<System.Windows.Forms.ToolTip> tooltipsList , string [] lines)
        {
            if (control == null)
                throw new ArgumentNullException(nameof(control));

            if (lines == null)
                throw new ArgumentNullException(nameof(control));

            System.Windows.Forms.ToolTip toolTip = new System.Windows.Forms.ToolTip
            {
                AutoPopDelay = 10000,   // how long the tooltip stays visible
                InitialDelay = 500,     // delay before it shows
                ReshowDelay = 200,      // delay between subsequent displays
                ShowAlways = true,      // show even if form is inactive
                UseFading = true,
                UseAnimation = true
            };

            // Join lines with Windows line breaks
            string tooltipText = string.Join(Environment.NewLine, lines);

            toolTip.SetToolTip(control, tooltipText);

            tooltipsList.Add(toolTip);
        }
    }
}
