import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';

import { AppComponent } from './app.component';
import { MainTemplateComponent } from './views/templates/main/main-template.component';
import {MomentModule} from "angular2-moment";
import {ViewDataService} from "./views/service/view-data.service";
import { MapViewComponent } from './views/map-view/map-view.component';
import { AgmCoreModule } from 'angular2-google-maps/core'
import {MR_Routing} from "./app.routing";
import { NavTemplateComponent } from './views/templates/nav/nav-template.component';
import {MainStatusComponent} from "./views/templates/main/main-status/main-status.component";
import {AppsViewComponent} from "./views/apps-view/apps-view.component";
import {AppItemComponent} from "./views/apps-view/app-item/app-item.component";
import { NavStatusComponent } from './views/templates/nav/nav-status/nav-status.component';
import {DateService} from "./views/service/date.service";
import { ClockViewComponent } from './views/clock-view/clock-view.component';
import { ControlViewComponent } from './views/control-view/control-view.component';
import {ControlSocketService} from "./views/service/control-socket.service";
import { PhoneViewComponent } from './views/phone-view/phone-view.component';
import { ContactsListComponent } from './views/phone-view/contacts-list/contacts-list.component';
import { DialPadComponent } from './views/phone-view/dial-pad/dial-pad.component';
import {ContactsService} from "./views/service/contacts.service";
import { ContactItemComponent } from './views/phone-view/contacts-list/contact-item/contact-item.component';
import {DialService} from "./views/service/dial.service";
import { NumberSelectComponent } from './views/phone-view/number-select/number-select.component';
import { OnCallComponent } from './views/phone-view/on-call/on-call.component';
import { MusicViewComponent } from './views/music-view/music-view.component';
import { SongsComponent } from './views/music-view/songs/songs.component';
import { MusicItemComponent } from './views/music-view/music-item/music-item.component';
import { MusicRecordComponent } from './views/music-view/music-records/music-record/music-record.component';
import { MusicRecordsComponent } from './views/music-view/music-records/music-records.component';
import {MusicService} from "./views/service/music.service";
import { RoadViewComponent } from './views/road-view/road-view.component';

@NgModule({
  declarations: [
    AppComponent,
    AppsViewComponent,
    MainStatusComponent,
    AppItemComponent,
    MainTemplateComponent,
    MapViewComponent,
    NavTemplateComponent,
    NavStatusComponent,
    ClockViewComponent,
    ControlViewComponent,
    PhoneViewComponent,
    ContactsListComponent,
    DialPadComponent,
    ContactItemComponent,
    NumberSelectComponent,
    OnCallComponent,
    MusicViewComponent,
    SongsComponent,
    MusicItemComponent,
    MusicRecordComponent,
    MusicRecordsComponent,
    RoadViewComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpModule,
    MomentModule,
    AgmCoreModule.forRoot({
      apiKey: 'AIzaSyBRGEmI4XxmYOsMpdY11ejqQf08ZCzI9cM'
    }),
    MR_Routing
  ],
  providers: [
    ViewDataService,
    DateService,
    ControlSocketService,
    ContactsService,
    DialService,
    MusicService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
