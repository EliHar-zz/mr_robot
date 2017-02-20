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

@NgModule({
  declarations: [
    AppComponent,
    AppsViewComponent,
    MainStatusComponent,
    AppItemComponent,
    MainTemplateComponent,
    MapViewComponent,
    NavTemplateComponent,
    NavStatusComponent
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
  providers: [ViewDataService, DateService],
  bootstrap: [AppComponent]
})
export class AppModule { }
