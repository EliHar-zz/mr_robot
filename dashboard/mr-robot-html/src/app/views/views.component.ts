import { Component, OnInit } from '@angular/core';
import {ViewDataService} from "./service/view-data.service";
import {AppItemClass} from "./home-view/apps-list/app-item/app-item";

@Component({
  selector: 'app-views',
  templateUrl: './views.component.html',
  styleUrls: ['./views.component.scss']
})
export class ViewsComponent implements OnInit {

  constructor(public viewDataService : ViewDataService) { }

  private _addDefaultViews() {
    this.viewDataService.addView(new AppItemClass("Map", "fa fa-map"));
    this.viewDataService.addView(new AppItemClass("Road", "fa fa-road"));
    this.viewDataService.addView(new AppItemClass("Phone", "fa fa-phone"));
    this.viewDataService.addView(new AppItemClass("Music", "fa fa-music"));
  }

  ngOnInit() {
    this._addDefaultViews();
  }
}
