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
	class InputProxyBehavior : Behavior<FrameworkElement>
	{
		Engine.MainDisplayer		DisplayerRef;

		protected override void OnAttached()
		{
			base.OnAttached();

			DisplayerRef = null;
			
			this.AssociatedObject.MouseDown += new MouseButtonEventHandler( MouseButtonStateChanged );
			this.AssociatedObject.MouseUp += new MouseButtonEventHandler( MouseButtonStateChanged );

			this.AssociatedObject.KeyDown += new KeyEventHandler( KeyStateChanged );
			this.AssociatedObject.KeyUp += new KeyEventHandler( KeyStateChanged );

			this.AssociatedObject.MouseMove += new MouseEventHandler( MouseMoved );
			this.AssociatedObject.MouseEnter += new MouseEventHandler( MouseEntered );
			this.AssociatedObject.MouseLeave += new MouseEventHandler( MouseLeft );
			this.AssociatedObject.PreviewMouseWheel += new MouseWheelEventHandler( MouseWheelMoved );

			this.AssociatedObject.LostFocus += new RoutedEventHandler( LostFocus );
			this.AssociatedObject.GotFocus += new RoutedEventHandler( GotFocus );
		}

		void MouseButtonStateChanged			( object sender, MouseButtonEventArgs e )
		{
			if( DisplayerRef == null )
				DisplayerRef = this.AssociatedObject.DataContext as Engine.MainDisplayer;

			DisplayerRef.EngineWrapper.MouseButtonChange( e.ChangedButton, e.ButtonState == MouseButtonState.Pressed );

			// Na wszelki wypadek zaktualizujmy pozycję myszy.
			var position = e.GetPosition( this.AssociatedObject );
			DisplayerRef.EngineWrapper.MousePositionChange( position.X, position.Y );
			e.Handled = true;
		}

		void KeyStateChanged					( object sender, KeyEventArgs e )
		{
			if( DisplayerRef == null )
				DisplayerRef = this.AssociatedObject.DataContext as Engine.MainDisplayer;

			DisplayerRef.EngineWrapper.KeyboardChange( e.Key, e.IsDown );
			e.Handled = true;
		}

		void GotFocus							( object sender, RoutedEventArgs e )
		{
			if( DisplayerRef == null )
				DisplayerRef = this.AssociatedObject.DataContext as Engine.MainDisplayer;

			DisplayerRef.GotFocus();
			e.Handled = true;
		}

		void LostFocus							( object sender, RoutedEventArgs e )
		{
			if( DisplayerRef == null )
				DisplayerRef = this.AssociatedObject.DataContext as Engine.MainDisplayer;

			DisplayerRef.LostFocus();
			e.Handled = true;
		}

		#region MouseEvents

		void MouseEntered						( object sender, MouseEventArgs e )
		{
			this.AssociatedObject.Focus();
			e.Handled = true;
		}

		void MouseLeft							( object sender, MouseEventArgs e )
		{
			Keyboard.ClearFocus();
			e.Handled = true;
		}

		void MouseMoved( object sender, MouseEventArgs e )
		{
			if( DisplayerRef == null )
				DisplayerRef = this.AssociatedObject.DataContext as Engine.MainDisplayer;

			var position = e.GetPosition( this.AssociatedObject );
			DisplayerRef.EngineWrapper.MousePositionChange( position.X, position.Y );

			e.Handled = true;
		}

		void MouseWheelMoved( object sender, MouseWheelEventArgs e )
		{
			if( DisplayerRef == null )
				DisplayerRef = this.AssociatedObject.DataContext as Engine.MainDisplayer;

			DisplayerRef.EngineWrapper.MouseWheelChange( (double)e.Delta );
			e.Handled = true;
		}

		#endregion

	}
}
