using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interactivity;
using System.Windows.Input;

namespace EditorApp.GUI
{
	class DeleteItemBehavior : Behavior<FrameworkElement>
	{

		protected override void OnAttached()
		{
			base.OnAttached();

			this.AssociatedObject.PreviewKeyDown += new KeyEventHandler( KeyDown );
		}


		void KeyDown( object sender, KeyEventArgs e )
		{
			if( e.Key == Key.Delete )
			{
				IDeletableItem view = this.AssociatedObject.DataContext as IDeletableItem;

				var property = this.AssociatedObject.GetType().GetProperty( "SelectedItem" );
				if( property != null )
				{
					view.DeleteItem( property.GetValue( this.AssociatedObject ) );
				}
			}
			
			e.Handled = true;
		}
	}
}
