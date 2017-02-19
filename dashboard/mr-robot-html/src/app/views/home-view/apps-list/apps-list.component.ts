import {Component, OnInit, Input} from '@angular/core';
import {AppItemClass} from "./app-item/app-item";
import {ViewDataService} from "../../service/view-data.service";

@Component({
  selector: 'app-apps-list',
  templateUrl: './apps-list.component.html',
  styleUrls: ['./apps-list.component.scss']
})
export class AppsListComponent implements OnInit {
  applications : AppItemClass[];
  constructor(private viewDataService : ViewDataService) {}

  onSelect(selectedApp : AppItemClass) {
    console.log("Selecting view: " + selectedApp.title);
  }

  ngOnInit() {
    this.applications = this.viewDataService.getViews();
  }
}
