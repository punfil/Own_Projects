import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, GLib
from datetime import datetime
import pytz
import sys


ENGLISH_TO_POLISH_DAYS = {
    "Mon": "Pon",
    "Tue": "Wt",
    "Wed": "Śr",
    "Thu": "Czw",
    "Fri": "Pt",
    "Sat": "Sob",
    "Sun": "Niedz"
}


class Alarm:
    def __init__(self, time, days, enabled=True):
        self.time = time
        self.days = days
        self.enabled = enabled


class AddAlarmDialog(Gtk.Dialog):
    def __init__(self, parent):
        Gtk.Dialog.__init__(self, "Dodaj nowy alarm", parent, 0,
                            (Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,
                             Gtk.STOCK_OK, Gtk.ResponseType.OK))

        self.set_modal(True)

        box = self.get_content_area()
        grid = Gtk.Grid()
        box.add(grid)

        self.time_edit = Gtk.Entry()
        grid.attach(Gtk.Label("Czas alarmu:"), 0, 0, 1, 1)
        grid.attach(self.time_edit, 1, 0, 1, 1)

        self.day_checkboxes = []
        days_of_week = ["Pon", "Wt", "Śr", "Czw", "Pt", "Sob", "Niedz"]
        for i, day in enumerate(days_of_week):
            checkbox = Gtk.CheckButton(day)
            grid.attach(checkbox, 1, i + 1, 1, 1)
            self.day_checkboxes.append(checkbox)

        self.enabled_checkbox = Gtk.CheckButton("Włączony")
        self.enabled_checkbox.set_active(True)
        grid.attach(self.enabled_checkbox, 1, len(days_of_week) + 1, 1, 1)

        self.show_all()

    def get_alarm(self):
        time = self.time_edit.get_text()
        days = [checkbox.get_label() for checkbox in self.day_checkboxes if checkbox.get_active()]
        enabled = self.enabled_checkbox.get_active()
        return Alarm(time, days, enabled)


class ClockAppGTK3(Gtk.Window):
    def __init__(self):
        super().__init__(title="184657 Panfil Wojciech - Języki skryptowe 3 - Zegar")
        self.connect("destroy", Gtk.main_quit)
        self.set_border_width(10)
        self.set_default_size(800, 600)

        # Initialize timezones and create main notebook
        self.timezones = pytz.common_timezones
        self.notebook = Gtk.Notebook()
        self.add(self.notebook)

        # Create Clock tab
        self.create_clock_tab()

        # Create Stopwatch tab
        self.create_stopwatch_tab()

        # Create Timer tab
        self.create_timer_tab()

        # Create Alarm tab
        self.create_alarm_tab()

        # Create menu bar
        self.create_menu()

        # Start updating time
        self.time_update_id = GLib.timeout_add_seconds(1, self.update_time)

    def create_clock_tab(self):
        clock_tab = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
        self.current_time_label = Gtk.Label()
        self.current_time_label.set_alignment(0.5, 0.5)
        self.current_time_label.set_markup('<span size="large"></span>')
        clock_tab.pack_start(self.current_time_label, True, True, 0)

        self.timezone_combo = Gtk.ComboBoxText()
        for timezone in self.timezones:
            self.timezone_combo.append_text(timezone)
        self.timezone_combo.connect("changed", self.update_time)
        clock_tab.pack_start(self.timezone_combo, False, False, 0)

        self.notebook.append_page(clock_tab, Gtk.Label(label="Zegar"))

    def create_stopwatch_tab(self):
        stopwatch_tab = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
        self.stopwatch_label = Gtk.Label(label="00:00:00")
        self.stopwatch_label.set_alignment(0.5, 0.5)
        stopwatch_tab.pack_start(self.stopwatch_label, True, True, 0)

        start_stopwatch_button = Gtk.Button(label="Rozpocznij")
        start_stopwatch_button.connect("clicked", self.start_stopwatch)
        stopwatch_tab.pack_start(start_stopwatch_button, False, False, 0)

        stop_stopwatch_button = Gtk.Button(label="Zatrzymaj")
        stop_stopwatch_button.connect("clicked", self.stop_stopwatch)
        stopwatch_tab.pack_start(stop_stopwatch_button, False, False, 0)

        reset_stopwatch_button = Gtk.Button(label="Wyczyść")
        reset_stopwatch_button.connect("clicked", self.reset_stopwatch)
        stopwatch_tab.pack_start(reset_stopwatch_button, False, False, 0)

        self.notebook.append_page(stopwatch_tab, Gtk.Label(label="Stoper"))

    def create_timer_tab(self):
        timer_tab = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
        self.timer_edit = Gtk.Entry()
        self.timer_edit.set_placeholder_text("hh:mm:ss")
        timer_tab.pack_start(self.timer_edit, True, True, 0)

        start_timer_button = Gtk.Button(label="Rozpocznij")
        start_timer_button.connect("clicked", self.start_timer)
        timer_tab.pack_start(start_timer_button, False, False, 0)

        stop_timer_button = Gtk.Button(label="Zatrzymaj")
        stop_timer_button.connect("clicked", self.stop_timer)
        timer_tab.pack_start(stop_timer_button, False, False, 0)

        reset_timer_button = Gtk.Button(label="Wyczyść")
        reset_timer_button.connect("clicked", self.reset_timer)
        timer_tab.pack_start(reset_timer_button, False, False, 0)

        self.notebook.append_page(timer_tab, Gtk.Label(label="Minutnik"))

    def create_alarm_tab(self):
        alarm_tab = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
        self.alarm_list = Gtk.ListBox()
        alarm_tab.pack_start(self.alarm_list, True, True, 0)

        add_alarm_button = Gtk.Button(label="Dodaj nowy alarm")
        add_alarm_button.connect("clicked", self.add_new_alarm_dialog)
        alarm_tab.pack_start(add_alarm_button, False, False, 0)

        remove_alarm_button = Gtk.Button(label="Usuń alarm")
        remove_alarm_button.connect("clicked", self.remove_selected_alarm)
        alarm_tab.pack_start(remove_alarm_button, False, False, 0)

        self.notebook.append_page(alarm_tab, Gtk.Label(label="Budzik"))

    def create_menu(self):
        menu_bar = Gtk.MenuBar()

        file_menu = Gtk.Menu()
        file_menu_exit = Gtk.MenuItem(label="Exit")
        file_menu_exit.connect("activate", self.on_menu_file_exit)
        file_menu.append(file_menu_exit)

        file_menu_item = Gtk.MenuItem(label="File")
        file_menu_item.set_submenu(file_menu)
        menu_bar.append(file_menu_item)

        help_menu = Gtk.Menu()
        help_menu_about = Gtk.MenuItem(label="About")
        help_menu_about.connect("activate", self.on_menu_help_about)
        help_menu.append(help_menu_about)

        help_menu_item = Gtk.MenuItem(label="Help")
        help_menu_item.set_submenu(help_menu)
        menu_bar.append(help_menu_item)

        vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=0)
        vbox.pack_start(menu_bar, False, False, 0)
        self.add(vbox)

    def update_time(self, widget):
        timezone = self.timezones[self.timezone_combo.get_active()]
        local_time = pytz.timezone(timezone).localize(datetime.datetime.now())
        local_time_str = local_time.strftime("%Y-%m-%d %H:%M:%S %Z%z")
        self.current_time_label.set_text(local_time_str)

    def start_stopwatch(self, button):
        if hasattr(self, 'stopwatch_timer_id'):
            return

        def update_stopwatch():
            elapsed_time = datetime.now() - self.stopwatch_start_time
            stopwatch_time = str(elapsed_time).split(".")[0]
            self.stopwatch_label.set_text(stopwatch_time)

        self.stopwatch_start_time = datetime.now()
        self.stopwatch_timer_id = GLib.timeout_add_seconds(1, update_stopwatch)

    def stop_stopwatch(self, button):
        if hasattr(self, 'stopwatch_timer_id'):
            GLib.source_remove(self.stopwatch_timer_id)
            del self.stopwatch_timer_id


    def reset_stopwatch(self, button):
        if hasattr(self, 'stopwatch_timer_id'):
            GLib.source_remove(self.stopwatch_timer_id)
            del self.stopwatch_timer_id

        self.stopwatch_label.set_text("00:00:00")


    def start_timer(self, button):
        if hasattr(self, 'timer_id'):
            return

        def update_timer():
            current_time = datetime.now().time()
            if current_time >= self.timer_end_time:
                self.stop_timer(None)
                self.reset_timer(None)
            else:
                remaining_time = (self.timer_end_time.hour * 3600 +
                                self.timer_end_time.minute * 60 +
                                self.timer_end_time.second) - \
                                (current_time.hour * 3600 +
                                current_time.minute * 60 +
                                current_time.second)
                display_time = str(datetime.timedelta(seconds=remaining_time))
                self.timer_edit.set_text(display_time)

        self.timer_end_time = self.timer_edit.get_time()
        self.timer_id = GLib.timeout_add_seconds(1, update_timer)


    def stop_timer(self, button):
        if hasattr(self, 'timer_id'):
            GLib.source_remove(self.timer_id)
            del self.timer_id


    def reset_timer(self, button):
        if hasattr(self, 'timer_id'):
            GLib.source_remove(self.timer_id)
            del self.timer_id
        self.timer_edit.set_text("00:00:00")


    def add_new_alarm_dialog(self, button):
        dialog = AddAlarmDialog(self)
        if dialog.run() == Gtk.ResponseType.OK:
            alarm = dialog.get_alarm()
            self.alarms.append(alarm)
            self.update_alarm_list()
        dialog.destroy()


    def remove_selected_alarm(self, button):
        selected_row = self.alarm_list.get_selected_row()
        if selected_row is not None:
            selected_alarm = self.alarms[selected_row]
            self.alarms.remove(selected_alarm)
            self.update_alarm_list()


    def check_alarms(self):
        current_time = datetime.now().time()
        current_day = datetime.now().strftime("%a")
        for alarm in self.alarms:
            if alarm.enabled and current_time >= alarm.time and ENGLISH_TO_POLISH_DAYS[current_day] in alarm.days:
                self.show_alarm_message(alarm)
                alarm.enabled = False
                self.update_alarm_list()


    def show_alarm_message(self, alarm):
        dialog = Gtk.MessageDialog(
            transient_for=self,
            flags=0,
            message_type=Gtk.MessageType.INFO,
            buttons=Gtk.ButtonsType.OK,
            text=f"Alarm {alarm.time.strftime('%H:%M:%S')}!"
        )
        dialog.run()
        dialog.destroy()

    def on_menu_file_exit(self, widget):
        Gtk.main_quit()

    def on_menu_help_about(self, widget):
        about_dialog = Gtk.AboutDialog()
        about_dialog.set_program_name("Zegar")
        about_dialog.set_version("1.0")
        about_dialog.set_authors(["Wojciech Panfil"])
        about_text = "Zegar\n\n" \
                        "Aplikacja pozwala na:\n" \
                        "- Wyświetlenie aktualnego czasu w różnych strefach czasowych.\n" \
                        "- Skorzystać ze stopera celem zmierzenia czasu.\n" \
                        "- Użyć minutnika do odmierzenia czasu. \n\n" \
                        "- Ustawić budzik. \n\n" \
                        "Stworzono używając PyGTK3. \n\n"\
                        "184657 Panfil Wojciech"

        about_dialog.set_comments(about_text)

        about_dialog.run()
        about_dialog.destroy()

def launch_gtk():
    win = ClockAppGTK3()
    win.connect("destroy", Gtk.main_quit)
    win.show_all()
    Gtk.main()

    sys.exit(0)


if __name__ == "__main__":
    launch_gtk()
