import {Routes, RouterModule} from "@angular/router";
import {MapViewComponent} from "./views/map-view/map-view.component";
import {MainTemplateComponent} from "./views/templates/main/main-template.component";
import {NavTemplateComponent} from "./views/templates/nav/nav-template.component";
import {AppsViewComponent} from "./views/apps-view/apps-view.component";
import {ClockViewComponent} from "./views/clock-view/clock-view.component";
import {ControlViewComponent} from "./views/control-view/control-view.component";
import {PhoneViewComponent} from "./views/phone-view/phone-view.component";
import {MusicViewComponent} from "./views/music-view/music-view.component";
import {MusicItemComponent} from "./views/music-view/music-item/music-item.component";
import {RoadViewComponent} from "./views/road-view/road-view.component";

const APP_ROUTES : Routes = [
  { path: '', redirectTo: 'view/home', pathMatch: 'full'},
  { path: 'view', component: MainTemplateComponent, children: [
    { path: 'home', component: AppsViewComponent}
  ]},
  { path: 'open', component: NavTemplateComponent, children: [
    { path: 'map', component: MapViewComponent},
    { path: 'phone', component: PhoneViewComponent},
    { path: 'clock', component: ClockViewComponent},
    { path: 'control', component: ControlViewComponent},
    { path: 'music', component: MusicViewComponent},
    { path: 'song/:index', component: MusicItemComponent},
    { path: 'road', component: RoadViewComponent}
  ]}
];

export const MR_Routing = RouterModule.forRoot(APP_ROUTES);
