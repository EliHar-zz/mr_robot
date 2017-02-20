import { Component, OnInit } from '@angular/core';
import { Location } from '@angular/common'

@Component({
  selector: 'mr-nav-status',
  templateUrl: './nav-status.component.html',
  styleUrls: ['./nav-status.component.scss']
})
export class NavStatusComponent implements OnInit {

  constructor(private _location: Location) { }

  ngOnInit() {
  }

  onBackClicked() {
    this._location.back();
  }
}
