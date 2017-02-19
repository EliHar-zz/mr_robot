import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';

import { AppComponent } from './app.component';
import { HomeViewComponent } from './views/home-view/home-view.component';
import { HomeStatusComponent } from './views/home-view/home-status/home-status.component';
import { AppsListComponent } from './views/home-view/apps-list/apps-list.component';
import { AppItemComponent } from './views/home-view/apps-list/app-item/app-item.component';
import { ViewsComponent } from './views/views.component';
import {MomentModule} from "angular2-moment";
import {ViewDataService} from "./views/service/view-data.service";
import { MapViewComponent } from './views/map-view/map-view.component';
import { AgmCoreModule } from 'angular2-google-maps/core'

@NgModule({
  declarations: [
    AppComponent,
    HomeViewComponent,
    HomeStatusComponent,
    AppsListComponent,
    AppItemComponent,
    ViewsComponent,
    MapViewComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpModule,
    MomentModule,
    AgmCoreModule.forRoot({
      apiKey: 'AIzaSyBRGEmI4XxmYOsMpdY11ejqQf08ZCzI9cM'
    })
  ],
  providers: [ViewDataService],
  bootstrap: [AppComponent]
})
export class AppModule { }
