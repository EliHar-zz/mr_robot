import {Component, OnInit, Input} from '@angular/core';
import {AppItemClass} from "./app-item";

@Component({
  selector: 'app-app-item',
  templateUrl: './app-item.component.html',
  styleUrls: ['./app-item.component.scss']
})
export class AppItemComponent implements OnInit {

  @Input() application : AppItemClass;
  constructor() { }

  onSelect(selectedApp : AppItemClass) {
    console.log(selectedApp);
  }

  ngOnInit() {
  }

}
