import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'mr-map-view',
  templateUrl: './map-view.component.html',
  styleUrls: ['./map-view.component.scss'],

})
export class MapViewComponent implements OnInit {
  lat: number = 45.5579564;
  lng: number = -73.8516443;

  constructor() { }

  ngOnInit() {
  }

}
