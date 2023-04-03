#define AP_SSID "" // имя точки доступа (WiFi)
#define AP_PASS "" // параль от точки доступа (WiFi)

#include <GyverPortal.h>
GyverPortal ui;

int i_1 = 0;
int i_2 = 0;
int i_3 = 0;
int i_4 = 0;
int i_5 = 0;
int i_6 = 0;
int i_7 = 0;
int i_8 = 0;
int i_9 = 0;
int i_10 = 0;
int i_11 = 0;
int i_12 = 0;
int i_13 = 0;
int i_14 = 0;
int i_15 = 0;
int i_16 = 0;

String y = "Стабильно";
String n = "Ошибка";
String p = "Лаборатория";
String num = "12";
String of = "Выкл";

void build() {
  GP.BUILD_BEGIN(GP_DARK);
  GP.PAGE_TITLE("TAHK");

  GP.UPDATE("c1,job_MGS-A6,lighting,temp,hud,pres,CO2,err_mode,err_chs, request_KomSer,position,volume_transp,volume_perekach,volume_save,LOS,lightning,tok,watt,giroscope,material_current,material_need,status_water,max_water,gercon_window,gercon_door,status_door,status_window,status_pump,status_wind,status_light");

  GP.UI_BODY();

  GP.GRID_RESPONSIVE(700);

  if (ui.uri("/")) {

    GP.TITLE("Панель управления");
    GP.HR(GP_VIOL_B);

    GP.BLOCK_BEGIN(GP_THIN, "100%", "Уведомления", GP_CYAN);
    GP.TABLE_BORDER(true);
    GP.TABLE_BEGIN();

    GP.TR();
    GP.TD();
    GP.LABEL("Аварийный режим");
    GP.TD();
    GP.LABEL("NAN", "err_mode");

    GP.TR();
    GP.TD();
    GP.LABEL("Режим ЧС");
    GP.TD();
    GP.LABEL("NAN", "err_chs");

    GP.TR();
    GP.TD();
    GP.LABEL("Запрос ЦС");
    GP.TD();
    GP.LABEL("NAN", "request_KomSer");

    GP.TABLE_END();
    GP.BLOCK_END();

    GP.TITLE("Динамика");
    GP.HR(GP_VIOL_B);
    GP.BLOCK_BEGIN(GP_THIN, "100%", "Управление роботом", GP_CYAN);


    GP.BOX_BEGIN(GP_CENTER);
    GP.LABEL("Геопозиция:");
    GP.LABEL("NAN", "position");
    GP.BOX_END();

    GP.HR();

    GP.BOX_BEGIN(GP_CENTER);
    GP.LABEL("Переместиться в");
    GP.BOX_END();
    GP.BOX_BEGIN(GP_CENTER);
    GP.BUTTON("btn_lab", "Лаборатория", "btn_lb", GP_VIOL);
    GP.BUTTON("btn_kom", "Командный пункт", "btn_kp", GP_VIOL);
    GP.BOX_END();

    GP.BLOCK_END();

    GP.TITLE("Лаборатория");
    GP.HR(GP_VIOL_B);
    GP.BLOCK_BEGIN(GP_THIN, "100%", "Управление датчиками", GP_CYAN);
    GP.TABLE_BORDER(true);
    GP.TABLE_BEGIN();

    GP.TR();
    GP.TD();
    GP.LABEL("MGS-A6");
    GP.TD();
    GP.SWITCH("job_MGS-A6", true, GP_VIOL);

    GP.TR();
    GP.TD();
    GP.LABEL("MGS-CLM60");
    GP.TD();
    GP.SWITCH("job_MGS-CLM60", true, GP_VIOL);

    GP.TR();
    GP.TD();
    GP.LABEL("MGS-WT1");
    GP.TD();
    GP.SWITCH("job_MGS-WT1", true, GP_VIOL);

    GP.TR();
    GP.TD();
    GP.LABEL("Поток воды");
    GP.TD();
    GP.SWITCH("job_water", true, GP_VIOL);

    GP.TR();
    GP.TD();
    GP.LABEL("MGS-D20");
    GP.TD();
    GP.SWITCH("job_MGS-D20", true, GP_VIOL);

    GP.TR();
    GP.TD();
    GP.LABEL("MGS-THP80");
    GP.TD();
    GP.SWITCH("job_MGS-THP80", true, GP_VIOL);

    GP.TR();
    GP.TD();
    GP.LABEL("MGS-L75");
    GP.TD();
    GP.SWITCH("job_MGS-L75", true, GP_VIOL);

    GP.TR();
    GP.TD();
    GP.LABEL("Датчик тока");
    GP.TD();
    GP.SWITCH("job_tok", true, GP_VIOL);

    GP.TR();
    GP.TD();
    GP.LABEL("MGS-CO30");
    GP.TD();
    GP.SWITCH("job_MGS-CO30", true, GP_VIOL);

    GP.TABLE_END();
    GP.BLOCK_END();

    GP.BLOCK_BEGIN(GP_THIN, "100%", "Показания датчиков", GP_CYAN);
    GP.TABLE_BORDER(true);
    GP.TABLE_BEGIN();

    GP.TR();
    GP.TD();
    GP.LABEL("Ресурс (нужный)");
    GP.TD();
    GPcolor color1(0, 0, 0);
    GP.COLOR("color_need", color1, true);

    GP.TR();
    GP.TD();
    GP.LABEL("Ресурс (текущий)");
    GP.TD();
    GP.COLOR("color_current", color1, true);

    GP.TR();
    GP.TD();
    GP.LABEL("Объем (трансп.)");
    GP.TD();
    GP.LABEL("NAN", "volume_transp");

    GP.TR();
    GP.TD();
    GP.LABEL("Статус перекачки");
    GP.TD();
    GP.LED("status_water");

    GP.TR();
    GP.TD();
    GP.LABEL("Объем (перекач.)");
    GP.TD();
    GP.LABEL("NAN", "volume_perekach");

    GP.TR();
    GP.TD();
    GP.LABEL("Объем (хранения)");
    GP.TD();
    GP.LABEL("NAN", "volume_save");

    GP.TR();
    GP.TD();
    GP.LABEL("Перелив");
    GP.TD();
    GP.LABEL("NAN", "max_water");

    GP.TR();
    GP.TD();
    GP.LABEL("ЛОС");
    GP.TD();
    GP.LABEL("NAN", "LOS");

    GP.TR();
    GP.TD();
    GP.LABEL("CO2");
    GP.TD();
    GP.LABEL("NAN", "CO2");

    GP.TR();
    GP.TD();
    GP.LABEL("Геркон+Окно");
    GP.TD();
    GP.LED("gercon_window");

    GP.TR();
    GP.TD();
    GP.LABEL("Геркон+Дверь");
    GP.TD();
    GP.LED("gercon_door");

    GP.TR();
    GP.TD();
    GP.LABEL("Освещенность");
    GP.TD();
    GP.LABEL("NAN", "lighting");

    GP.TR();
    GP.TD();
    GP.LABEL("Датчик тока");
    GP.TD();
    GP.LABEL("NAN", "tok");

    GP.TR();
    GP.TD();
    GP.LABEL("Ватт");
    GP.TD();
    GP.LABEL("NAN", "watt");

    GP.TR();
    GP.TD();
    GP.LABEL("Гироскоп");
    GP.TD();
    GP.LABEL("num", "giroscope");

    GP.TR();
    GP.TD();
    GP.LABEL("Давление");
    GP.TD();
    GP.LABEL("NAN", "pres");

    GP.TR();
    GP.TD();
    GP.LABEL("Влажноть");
    GP.TD();
    GP.LABEL("NAN", "hud");

    GP.TR();
    GP.TD();
    GP.LABEL("Температура");
    GP.TD();
    GP.LABEL("NAN", "temp");

    GP.TABLE_END();
    GP.BLOCK_END();

    GP.BLOCK_BEGIN(GP_THIN, "100%", "Дополнительные функции", GP_CYAN);
    GP.TABLE_BORDER(true);
    GP.TABLE_BEGIN();

    GP.TR();
    GP.TD();
    GP.LABEL("Умное освещение");
    GP.TD();
    GP.SWITCH("work_light", true, GP_VIOL);

    GP.TR();
    GP.TD();
    GP.LABEL("Помпа");
    GP.TD();
    GP.SWITCH("work_pump", true, GP_VIOL);

    GP.TR();
    GP.TD();
    GP.LABEL("Вентилятор");
    GP.TD();
    GP.SWITCH("work_wind", true, GP_VIOL);

    GP.TR();
    GP.TD();
    GP.LABEL("Сервопривод (окно)");
    GP.TD();
    GP.SWITCH("work_servo-window", true, GP_VIOL);

    GP.TR();
    GP.TD();
    GP.LABEL("Сервопривод (дверь)");
    GP.TD();
    GP.SWITCH("work_servo-door", true, GP_VIOL);

    GP.TR();
    GP.TD();
    GP.LABEL("Светодиодная лента");
    GP.TD();
    GP.SWITCH("work_lamp", true, GP_VIOL);

    GP.TABLE_END();
    GP.BLOCK_END();

    GP.TITLE("Обслуживание");
    GP.HR(GP_VIOL_B);

    GP.BLOCK_BEGIN(GP_THIN, "100%", "Статус датчиков", GP_CYAN);
    GP.TABLE_BORDER(true);
    GP.TABLE_BEGIN();

    GP.TR();
    GP.TD();
    GP.LABEL("Дверь");
    GP.TD();
    GP.LED("status_door");

    GP.TR();
    GP.TD();
    GP.LABEL("Окно");
    GP.TD();
    GP.LED("status_window");

    GP.TR();
    GP.TD();
    GP.LABEL("Помпа");
    GP.TD();
    GP.LED("status_pump");

    GP.TR();
    GP.TD();
    GP.LABEL("Вентилятор");
    GP.TD();
    GP.LED("status_wind");

    GP.TR();
    GP.TD();
    GP.LABEL("Светодиодная лента");
    GP.TD();
    GP.LED("status_light");

    GP.TABLE_END();
    GP.BLOCK_END();

    GP.BUTTON("btn_service", "Выполнить проверку датчиков", "btn_check", GP_VIOL);
  }

  GP.UI_END();    // завершить окно панели управления <<<<-------------
  GP.BUILD_END();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  ui.attachBuild(build);
  ui.attach(action);
  ui.start();

  ui.enableAuth("admin", "1234");
}

void action() {
  if (ui.click()) {
    if (ui.click("c1")) Serial.println(ui.getString());
    if (ui.click("btn_lab")) {
      Serial.println("btn to lab");
    } else if (ui.click("btn_kom")) {
      Serial.println("btn to komand punkt");
    } else if (ui.click("btn_service")) {
      i_16 = 1;
      Serial.println("btn to service");
    } else if (ui.click("job_MGS-A6")) {
      if (i_1 == 1) {
        //if on
        Serial.println("switch job_MGS-A6 - on");

        i_1 = 0;
      } else {
        Serial.println("switch job_MGS-A6 - off");
        i_1 = 1;
      }
    } else if (ui.click("job_MGS-CLM60")) {
      if (i_2 == 1) {
        //if on
        Serial.println("switch job_MGS-CLM60 - on");
        i_2 = 0;
      } else {
        Serial.println("switch job_MGS-CLM60 - off");
        i_2 = 1;
      }
    } else if (ui.click("job_MGS-WT1")) {
      if (i_3 == 1) {
        //if on
        Serial.println("switch job_MGS-WT1 - on");
        i_3 = 0;
      } else {
        Serial.println("switch job_MGS-WT1 - off");
        i_3 = 1;
      }
    } else if (ui.click("job_water")) {
      if (i_4 == 1) {
        //if on
        Serial.println("switch job_water - on");
        i_4 = 0;
      } else {
        Serial.println("switch job_water - off");
        i_4 = 1;
      }
    } else if (ui.click("job_MGS-D20")) {
      if (i_5 == 1) {
        //if on
        Serial.println("switch job_MGS-D20 - on");
        i_5 = 0;
      } else {
        Serial.println("switch job_MGS-D20 - off");
        i_5 = 1;
      }
    } else if (ui.click("job_MGS-THP80")) {
      if (i_6 == 1) {
        Serial.println("switch job_MGS-THP80 - on");
        i_6 = 0;
      } else {
        Serial.println("switch job_MGS-THP80 - off");
        i_6 = 1;
      }
    } else if (ui.click("job_MGS-L75")) {
      if (i_7 == 1) {
        Serial.println("switch job_MGS-L75 - on");
        i_7 = 0;
      } else {
        Serial.println("switch job_MGS-L75 - off");
        i_7 = 1;
      }
    } else if (ui.click("job_tok")) {
      if (i_8 == 1) {
        Serial.println("switch job_tok - on");
        i_8 = 0;
      } else {
        Serial.println("switch job_tok - off");
        i_8 = 1;
      }
    } else if (ui.click("job_MGS-CO30")) {
      if (i_9 == 1) {
        Serial.println("switch job_MGS-CO30 - on");
        i_9 = 0;
      } else {
        Serial.println("switch job_MGS-CO30 - off");
        i_9 = 1;
      }
    } else if (ui.click("work_pump")) {
      if (i_10 == 1) {
        Serial.println("switch work_pump - on");
        i_10 = 0;
      } else {
        Serial.println("switch work_pump - off");
        i_10 = 1;
      }
    } else if (ui.click("work_light")) {
      if (i_11 == 1) {
        Serial.println("switch work_light - on");
        i_11 = 0;
      } else {
        Serial.println("switch work_light - off");
        i_11 = 1;
      }
    } else if (ui.click("work_wind")) {
      if (i_12 == 1) {
        //if on
        Serial.println("switch work_wind - on");
        i_12 = 0;
      } else {
        Serial.println("switch work_wind - off");
        i_12 = 1;
      }
    } else if (ui.click("work_servo-window")) {
      if (i_13 == 1) {
        //if on
        Serial.println("switch work_servo-window - on");
        i_13 = 0;
      } else {
        Serial.println("switch work_servo-window - off");
        i_13 = 1;
      }
    } else if (ui.click("work_servo-door")) {
      if (i_14 == 1) {
        //if on
        Serial.println("switch work_servo-door - on");
        i_14 = 0;
      } else {
        Serial.println("switch work_servo-door - off");
        i_14 = 1;
      }
    } else if (ui.click("work_lamp")) {
      if (i_15 == 1) {
        //if on
        Serial.println("switch work_lamp - on");
        i_15 = 0;
      } else {
        Serial.println("switch work_lamp - off");
        i_15 = 1;
      }
    }
  }

  if (ui.update()) {
    ui.updateString("err_mode", y);
    ui.updateString("err_chs", y);
    ui.updateString("request_KomSer", n);
    if (i_4 == 0) {
      ui.updateString("volume_perekach", num);
    } else {
      ui.updateString("volume_perekach", of);
    }
    ui.updateString("position", p);
    if (i_5 == 0) {
      ui.updateString("volume_transp", num);
    } else {
      ui.updateString("volume_transp", of);
    }
    if (i_9 == 0) {
      ui.updateString("LOS", num);
      ui.updateString("CO2", num);
    } else {
      ui.updateString("LOS", of);
      ui.updateString("CO2", of);
    }
    if (i_7 == 0) {
      ui.updateString("lighting", num);
    } else {
      ui.updateString("lighting", of);
    }
    if (i_8 == 0) {
      ui.updateString("tok", num);
      ui.updateString("watt", num);
    } else {
      ui.updateString("tok", of);
      ui.updateString("watt", of);
    }

    if (i_1 == 0) {
      ui.updateString("giroscope", num);
    } else {
      ui.updateString("giroscope", of);
    }
    if (i_6 == 0) {
      ui.updateString("pres", num);
      ui.updateString("hud", num);
      ui.updateString("temp", num);
    } else {
      ui.updateString("pres", of);
      ui.updateString("hud", of);
      ui.updateString("temp", of);
    }
    if (i_2 == 0) {
      if (ui.update("material_current")) ui.answer("Ожидание");
    } else {
      if (ui.update("material_current")) ui.answer(of);
    }
    if (i_10 == 0) {
      if (ui.update("status_water")) ui.answer(true);
    } else {
      if (ui.update("status_water")) ui.answer(false);
    }
    if (i_3 == 0) {
      if (ui.update("max_water")) ui.answer("Работает");
      ui.updateString("volume_save", num);
    } else {
      if (ui.update("max_water")) ui.answer(of);
      ui.updateString("volume_save", of);
    }
    if (i_13 == 0) {
      if (ui.update("gercon_window")) ui.answer(true);
    } else {
      if (ui.update("gercon_window")) ui.answer(false);
    }

    if (i_14 == 0) {
      if (ui.update("gercon_door")) ui.answer(true);
    } else {
      if (ui.update("gercon_door")) ui.answer(false);
    }
  
    if (ui.update("status_door")) ui.answer(0);
    if (ui.update("status_window")) ui.answer(0);
    if (ui.update("status_pump")) ui.answer(0);
    if (ui.update("status_wind")) ui.answer(0);
    if (ui.update("status_light")) ui.answer(0);
  }
}

void loop() {
  ui.tick();
}