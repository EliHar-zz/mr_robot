import {Component, OnInit, Input} from '@angular/core';
import {AppItemClass} from "./app-item";

@Component({
  selector: 'mr-app-item',
  templateUrl: './app-item.component.html',
  styleUrls: ['./app-item.component.scss']
})
export class AppItemComponent implements OnInit {

  @Input() application : AppItemClass;
  constructor() { }

  ngOnInit() {
  }

}
