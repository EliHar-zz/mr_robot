import {Component, OnInit, Input} from '@angular/core';
import {MusicItem} from "../music-item/music-item";

@Component({
  selector: 'mr-music-records',
  templateUrl: './music-records.component.html',
  styleUrls: ['./music-records.component.scss']
})
export class MusicRecordsComponent implements OnInit {

  @Input() songs : MusicItem[];
  constructor() { }

  ngOnInit() {

  }
}
