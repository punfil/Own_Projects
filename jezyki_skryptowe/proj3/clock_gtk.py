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

class Stopwatch:
    def __init__(self, window):
        self.stopwatch_start_time = datetime.now()
        self.stopwatch_timer_id = GLib.timeout_add_seconds(1, self.update_stopwatch)
        self.window = window
        self.enabled = True

    def update_stopwatch(self):
        if self.enabled == False:
            return False
        elapsed_time = datetime.now() - self.stopwatch_start_time
        stopwatch_time = "0" + str(elapsed_time).split(".")[0]
        self.window.stopwatch_label.set_text(stopwatch_time)

        return True


class Timer:
    def __init__(self, window, timer_end_time):
        self.enabled = True
        self.window = window
        self.time_counted = None
        self.timer_end_time = timer_end_time
        self.timer = GLib.timeout_add_seconds(1, self.update_timer)

    def update_timer(self):
        if self.enabled :
            self.time_counted += 1
        if g_timer is None:
            return False
        
        if self.time_counted >= self.timer_end_time:
            self.window.reset_timer(None)
        else:
            total_seconds = self.timer_end_time - self.time_counted
            hours, remainder = divmod(total_seconds, 3600)
            minutes, seconds = divmod(remainder, 60)
            time_str = "{:02}:{:02}:{:02}".format(int(hours), int(minutes), int(seconds))
            self.window.timer_edit.set_text(time_str)
        return True

g_stopwatch = None
g_timer = None
g_alarms = []


class AddAlarmDialog(Gtk.Dialog):
    def __init__(self, parent):
        Gtk.Dialog.__init__(self, "Dodaj nowy alarm", parent, 0, None)
        self.add_button(Gtk.STOCK_OK, Gtk.ResponseType.OK)
        self.add_button("Anuluj", Gtk.ResponseType.CANCEL)

        box = self.get_content_area()
        grid = Gtk.Grid()
        box.add(grid)

        self.time_edit = Gtk.Entry()
        self.time_edit.set_placeholder_text("00:00:00")
        self.time_edit.set_text("00:00:00")
        grid.attach(Gtk.Label("Czas alarmu:"), 0, 0, 1, 1)
        grid.attach(self.time_edit, 1, 0, 1, 1)

        self.day_checkboxes = []
        days_of_week = ["Pon", "Wt", "Śr", "Czw", "Pt", "Sob", "Niedz"]
        for i, day in enumerate(days_of_week):
            checkbox = Gtk.CheckButton(day)
            grid.attach(checkbox, 1, i + 1, 1, 1)
            self.day_checkboxes.append(checkbox)

        empty_label = Gtk.Label()
        empty_label.set_hexpand(True)
        grid.attach(empty_label, 1, len(days_of_week) + 1, 1, 1)

        self.enabled_checkbox = Gtk.CheckButton("Włączony")
        self.enabled_checkbox.set_active(True)
        grid.attach(self.enabled_checkbox, 1, len(days_of_week) + 2, 1, 1)

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

        self.main_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        self.notebook = Gtk.Notebook()

        # Clock Tab
        self.clock_tab = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
        self.current_time_label = Gtk.Label()
        self.current_time_label.set_alignment(0.5, 0.5)
        self.current_time_label.set_markup('<span size="large"></span>')
        self.clock_tab.pack_start(self.current_time_label, True, True, 0)

        self.timezone_combo = Gtk.ComboBoxText()
        self.timezones = pytz.common_timezones
        for timezone in self.timezones:
            self.timezone_combo.append_text(timezone)
        self.timezone_combo.connect("changed", self.update_time)
        self.timezone_combo.set_active(11)
        self.clock_tab.pack_start(self.timezone_combo, False, False, 0)

        self.notebook.append_page(self.clock_tab, Gtk.Label(label="Zegar"))

        # Stopwatch Tab
        self.stopwatch_tab = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
        self.stopwatch_label = Gtk.Label(label="00:00:00")
        self.stopwatch_label.set_alignment(0.5, 0.5)
        self.stopwatch_tab.pack_start(self.stopwatch_label, True, True, 0)

        self.start_stopwatch_button = Gtk.Button(label="Rozpocznij")
        self.stopwatch_tab.pack_start(self.start_stopwatch_button, False, False, 0)
        self.start_stopwatch_button.connect("clicked", self.start_stopwatch)

        self.stop_stopwatch_button = Gtk.Button(label="Zatrzymaj")
        self.stopwatch_tab.pack_start(self.stop_stopwatch_button, False, False, 0)
        self.stop_stopwatch_button.connect("clicked", self.stop_stopwatch)

        self.reset_stopwatch_button = Gtk.Button(label="Wyczyść")
        self.stopwatch_tab.pack_start(self.reset_stopwatch_button, False, False, 0)
        self.reset_stopwatch_button.connect("clicked", self.reset_stopwatch)

        self.notebook.append_page(self.stopwatch_tab, Gtk.Label(label="Stoper"))

        # Timer Tab
        self.timer_tab = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
        self.timer_edit = Gtk.Entry()
        self.timer_edit.set_placeholder_text("00:00:00")
        self.timer_tab.pack_start(self.timer_edit, True, True, 0)

        self.start_timer_button = Gtk.Button(label="Rozpocznij")
        self.timer_tab.pack_start(self.start_timer_button, False, False, 0)
        self.start_timer_button.connect("clicked", self.start_timer)

        self.stop_timer_button = Gtk.Button(label="Zatrzymaj")
        self.timer_tab.pack_start(self.stop_timer_button, False, False, 0)
        self.stop_timer_button.connect("clicked", self.stop_timer)

        self.reset_timer_button = Gtk.Button(label="Wyczyść")
        self.timer_tab.pack_start(self.reset_timer_button, False, False, 0)
        self.reset_timer_button.connect("clicked", self.reset_timer)

        self.notebook.append_page(self.timer_tab, Gtk.Label(label="Minutnik"))

        # Alarm Tab
        self.alarm_tab = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
        self.alarm_list = Gtk.ListBox()
        self.alarm_list.connect("row-activated", self.on_alarm_activated)
        self.alarm_tab.pack_start(self.alarm_list, True, True, 0)

        self.add_alarm_button = Gtk.Button(label="Dodaj nowy alarm")
        self.add_alarm_button.connect("clicked", self.add_new_alarm_dialog)
        self.alarm_tab.pack_start(self.add_alarm_button, False, False, 0)

        self.remove_alarm_button = Gtk.Button(label="Usuń alarm")
        self.remove_alarm_button.connect("clicked", self.remove_selected_alarm)
        self.alarm_tab.pack_start(self.remove_alarm_button, False, False, 0)

        self.notebook.append_page(self.alarm_tab, Gtk.Label(label="Budzik"))

        self.menu_bar = Gtk.MenuBar()
        vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=0)
        vbox.pack_start(self.menu_bar, False, False, 0)

        file_menu = Gtk.Menu()
        file_menu_exit = Gtk.MenuItem(label="Wyjdź")
        file_menu_exit.connect("activate", self.on_menu_file_exit)
        file_menu.append(file_menu_exit)

        file_menu_item = Gtk.MenuItem(label="Plik")
        file_menu_item.set_submenu(file_menu)
        self.menu_bar.append(file_menu_item)

        help_menu = Gtk.Menu()
        help_menu_about = Gtk.MenuItem(label="O aplikacji")
        help_menu_about.connect("activate", self.on_menu_help_about)
        help_menu.append(help_menu_about)

        help_menu_item = Gtk.MenuItem(label="Pomoc")
        help_menu_item.set_submenu(help_menu)
        self.menu_bar.append(help_menu_item)

        self.time_update_id = GLib.timeout_add_seconds(1, self.update_time, None)

        self.main_box.pack_start(vbox, False, False, 0)
        self.main_box.pack_start(self.notebook, False, False, 0)
        self.add(self.main_box)

        self.alarms = []
        self.number_of_clicks = 0

    def update_time(self, combo):
        timezone_index = self.timezone_combo.get_active()

        current_time = datetime.now(pytz.timezone(pytz.common_timezones[timezone_index]))
        local_time_str = current_time.strftime('%Y-%m-%d %H:%M:%S %Z%z')
        self.current_time_label.set_markup('<span size="large">{}</span>'.format(local_time_str))
        return True

    def start_stopwatch(self, _):
        global g_stopwatch
        if g_stopwatch is not None:
            return

        g_stopwatch = Stopwatch(self)

    def stop_stopwatch(self, _):
        global g_stopwatch
        if g_stopwatch is not None:
            self.stopwatch_label.set_text("00:00:00")
            g_stopwatch.enabled = False
            g_stopwatch = None


    def reset_stopwatch(self, _):
        self.stop_stopwatch(None)
        self.stopwatch_label.set_text("00:00:00")


    def start_timer(self, button):
        global g_timer

        if g_timer:
            g_timer.enabled = True
            return

        g_timer = Timer(self, self.get_time(self.timer_edit.get_text()))
        g_timer.enabled = True
        g_timer.time_counted = self.get_time("00:00:00")

    def get_time(self, time_str):
        t = datetime.strptime(time_str, "%H:%M:%S").time()
        return t.hour * 3600 + t.minute * 60 + t.second

    def stop_timer(self, button):
        global g_timer
        g_timer.enabled = False


    def reset_timer(self, button):
        global g_timer
        g_timer.enabled = False
        g_timer = None
        self.timer_edit.set_text("")


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
            selected_alarm = self.alarms[selected_row.get_index()]
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


    def edit_alarm_dialog(self, alarm):
        dialog = Gtk.Dialog(
            "Edytuj alarm",
            self,
            Gtk.DialogFlags.MODAL | Gtk.DialogFlags.DESTROY_WITH_PARENT,
            (
                "Anuluj",
                Gtk.ResponseType.CANCEL,
                Gtk.STOCK_OK,
                Gtk.ResponseType.OK,
            ),
        )
        dialog.set_default_size(200, 200)

        content_area = dialog.get_content_area()
        vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=6)
        content_area.add(vbox)

        time_label = Gtk.Label(label="Czas alarmu:")
        vbox.pack_start(time_label, False, False, 0)

        time_entry = Gtk.Entry()
        time_entry.set_text(alarm.time)
        vbox.pack_start(time_entry, False, False, 0)

        days_label = Gtk.Label(label="Dni tygodnia:")
        vbox.pack_start(days_label, False, False, 0)

        days_checkboxes = []
        days_of_week = ["Pon", "Wt", "Śr", "Czw", "Pt", "Sob", "Niedz"]
        for day in days_of_week:
            checkbox = Gtk.CheckButton(label=day)
            if day in alarm.days:
                checkbox.set_active(True)
            vbox.pack_start(checkbox, False, False, 0)
            days_checkboxes.append(checkbox)

        enabled_checkbox = Gtk.CheckButton(label="Włączony" if alarm.enabled else "Wyłączony")
        vbox.pack_start(enabled_checkbox, False, False, 0)

        dialog.show_all()

        response = dialog.run()

        if response == Gtk.ResponseType.OK:
            # Update alarm with new settings
            new_time = datetime.strptime(time_entry.get_text(), "%H:%M:%S").time()
            new_days = [day for day, checkbox in zip(days_of_week, days_checkboxes) if checkbox.get_active()]
            new_enabled = enabled_checkbox.get_active()

            alarm.time = new_time
            alarm.days = new_days
            alarm.enabled = new_enabled

            # Update alarm list
            self.update_alarm_list()

        dialog.destroy()

    def on_alarm_activated(self, widget, row):
        self.number_of_clicks += 1
        if self.number_of_clicks == 1:
            return
        
        self.number_of_clicks = 0
        alarm = self.alarms[row.get_index()]
        self.edit_alarm_dialog(alarm)


    def update_alarm_list(self):
        for row in self.alarm_list.get_children():
            self.alarm_list.remove(row)
        for alarm in self.alarms:
            alarm_text = f"{alarm.time} - {', '.join(alarm.days)} ({'Włączony' if alarm.enabled else 'Wyłączony'})"
            alarm_row = Gtk.ListBoxRow()
            alarm_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=10)
            alarm_label = Gtk.Label(label=alarm_text)
            alarm_box.pack_start(alarm_label, True, True, 0)
            alarm_row.add(alarm_box)
            self.alarm_list.add(alarm_row)
        self.alarm_list.show_all()

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
        about_dialog = Gtk.Dialog("O aplikacji",
                              None,
                              0,
                              None)
        # Customize the About dialog
        about_dialog.set_default_size(300, 200)
        about_dialog.set_border_width(10)

        about_text = "Zegar\n\n" \
                        "Aplikacja pozwala na:\n" \
                        "- Wyświetlenie aktualnego czasu w różnych strefach czasowych.\n" \
                        "- Skorzystać ze stopera celem zmierzenia czasu.\n" \
                        "- Użyć minutnika do odmierzenia czasu. \n\n" \
                        "- Ustawić budzik. \n\n" \
                        "Stworzono używając PyGTK3. \n\n"\
                        "184657 Panfil Wojciech"

        label = Gtk.Label(about_text)
        label.set_line_wrap(True)
        about_dialog.vbox.pack_start(label, True, True, 0)

        # Show the About dialog
        about_dialog.show_all()

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
