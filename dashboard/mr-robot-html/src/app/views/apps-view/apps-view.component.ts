import {Component, OnInit} from '@angular/core';
import {AppItemClass} from "./app-item/app-item";
import {ViewDataService} from "../service/view-data.service";

@Component({
  selector: 'mr-apps-view',
  templateUrl: './apps-view.component.html',
  styleUrls: ['./apps-view.component.scss']
})
export class AppsViewComponent implements OnInit {
  applications : AppItemClass[];
  constructor(private viewDataService : ViewDataService) {}

  onSelect(selectedApp : AppItemClass) {
    console.log("Selecting view: " + selectedApp.title);
  }

  ngOnInit() {
    this.applications = this.viewDataService.getViews();
  }
}
