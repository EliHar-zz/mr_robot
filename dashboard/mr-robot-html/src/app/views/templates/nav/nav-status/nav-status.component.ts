import { Component, OnInit } from '@angular/core';
import { Location } from '@angular/common'
import {DateService} from "../../../service/date.service";

@Component({
  selector: 'mr-nav-status',
  templateUrl: './nav-status.component.html',
  styleUrls: ['./nav-status.component.scss']
})
export class NavStatusComponent implements OnInit {

  constructor(private _location: Location, public dateService : DateService) { }

  ngOnInit() {
  }

  onBackClicked() {
    this._location.back();
  }
}
