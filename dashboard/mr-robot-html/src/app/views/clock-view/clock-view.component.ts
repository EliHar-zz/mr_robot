import { Component, OnInit } from '@angular/core';
import {DateService} from "../service/date.service";

@Component({
  selector: 'mr-clock-view',
  templateUrl: './clock-view.component.html',
  styleUrls: ['./clock-view.component.scss']
})
export class ClockViewComponent implements OnInit {

  constructor(public dateService : DateService) { }

  ngOnInit() {
  }

}
