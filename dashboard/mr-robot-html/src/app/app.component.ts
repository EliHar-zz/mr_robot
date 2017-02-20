import {Component, OnInit} from '@angular/core';
import {ViewDataService} from "./views/service/view-data.service";
import {AppItemClass} from "./views/home-view/apps-list/app-item/app-item";

@Component({
  selector: 'mr-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {

  constructor(public viewDataService : ViewDataService){}

  private _addDefaultViews() {
    this.viewDataService.addView(new AppItemClass("Map", "fa fa-map", ['/open','map']));
    this.viewDataService.addView(new AppItemClass("Road", "fa fa-road", ['/open', 'road']));
    this.viewDataService.addView(new AppItemClass("Phone", "fa fa-phone", ['/open', 'phone']));
    this.viewDataService.addView(new AppItemClass("Music", "fa fa-music", ['/open', 'music']));
  }

  ngOnInit() {
    this._addDefaultViews();
  }
}
