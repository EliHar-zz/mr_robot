import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'mr-main-status',
  templateUrl: './main-status.component.html',
  styleUrls: ['./main-status.component.scss']
})
export class MainStatusComponent implements OnInit {

  myDate: Date;
  constructor() { }

  ngOnInit() {
    this.myDate = new Date();
  }

}
