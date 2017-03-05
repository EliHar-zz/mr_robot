import {Component, OnInit, Input} from '@angular/core';
import {MusicItem} from "../../music-item/music-item";

@Component({
  selector: 'mr-music-record',
  templateUrl: './music-record.component.html',
  styleUrls: ['./music-record.component.scss']
})
export class MusicRecordComponent implements OnInit {

  @Input() song : MusicItem;
  constructor() { }

  ngOnInit() {
  }
}
