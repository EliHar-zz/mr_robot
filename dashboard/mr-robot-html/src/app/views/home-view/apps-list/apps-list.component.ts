import { Component, OnInit } from '@angular/core';
import {AppItemClass} from "./app-item/app-item";

@Component({
  selector: 'app-apps-list',
  templateUrl: './apps-list.component.html',
  styleUrls: ['./apps-list.component.scss']
})
export class AppsListComponent implements OnInit {
  applications : AppItemClass[];
  constructor() {}

  ngOnInit() {
    this.applications = [
      new AppItemClass("Map", "fa fa-map"),
      new AppItemClass("Road", "fa fa-road"),
      new AppItemClass("Phone", "fa fa-phone"),
      new AppItemClass("Music", "fa fa-music")
    ];
  }

}
